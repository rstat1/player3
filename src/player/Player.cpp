/*
* Copyright (c) 2017 An Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <SDL.h>
#include <chrono>
#include <thread>
#include <sched.h>
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <player/Player.h>
#include <ProfilerDefines.h>
#include <player/chat/ChatService.h>
#include <platform/PlatformManager.h>
#include <base/platform/linux/memtrack.h>

#include <ui/ember/EmberService.h>

using namespace std;
using namespace player3;
using namespace player3::ember;
using namespace player3::chat;
using namespace base::platform;
using namespace player3::platform;

#define AV_SYNC_THRESHOLD 0.01

namespace player3 { namespace player
{
	double Player::lastMemoryUse;
	std::shared_ptr<Player> Player::ref;
	PlatformInterface* Player::platformInterface;

	Player::Player() {}
	void Player::InitPlayer()
	{
		Log("Player", "player3 player init");
		av_log_set_level(AV_LOG_VERBOSE);
		av_register_all();
		avcodec_register_all();
		avformat_network_init();

		this->state = new InternalPlayerState();
		this->state->videoClock = 0;
		this->state->lastVideoPts = 0;
		this->state->lastAudioPts = 0;
		this->state->status = PlayerStatus::Stopped;
		this->state->audioState = new AudioState();
		this->state->audioState->audioClock = 0;
		this->state->bufferSignal = new ConditionVariable();

		platformInterface = PlatformManager::Get()->GetPlatformInterface();
		//platformInterface->DecoderReset();
		this->InitOverlay();
		this->SetEmberEventHandlers();

		HANDLE_EVENT(Connected, true, "UI", HANDLER {
			ChatService::Get()->JoinChannel("rstat1");
		});

#if defined(OS_LINUX) && !defined(OS_STEAMLINK)
		signal(SIGTERM, Player::SigTermHandler);
#endif
	}
	void Player::InitOverlay()
	{
		this->state->overlay = new InfoOverlayNUI();
		//this->state->overlay->AddStringValue("GitBranch", BranchName);
		this->state->overlay->AddDoubleValue("AVDelayDiff", 0);
		this->state->overlay->AddDoubleValue("MemCurrent(in MB)", MemTrack::GetCurrentMemoryUse());
		this->state->overlay->AddDoubleValue("FrameTimer", 0);
		this->state->overlay->AddDoubleValue("AudioDelay", 0.0);
		this->state->overlay->AddDoubleValue("AudioClock", 0);
		this->state->overlay->AddDoubleValue("VideoActualDelay", 0);
		this->state->overlay->AddDoubleValue("AVClockDiff", 0);
		this->state->overlay->AddIntValue("QueuedVideo", 0);
		this->state->overlay->AddIntValue("NextRefresh", 0);

		EVENT("UpdateOverlay");
		EVENT("StreamStarting");

		HANDLE_EVENT(UpdateOverlay, true, "PlayerApp", HANDLER {
			InternalPlayerState* playerState = (InternalPlayerState*)args;
			double currentUse = MemTrack::GetCurrentMemoryUse();
			if (currentUse != lastMemoryUse)
			{
				playerState->overlay->UpdateDoubleValue("MemPeak (in MB)", MemTrack::GetPeakMemoryUse());
				playerState->overlay->UpdateDoubleValue("MemCurrent (in MB)", MemTrack::GetCurrentMemoryUse());
				lastMemoryUse = currentUse;
			}
//			playerState->overlay->UpdateStringValue("GitBranch", BranchName);
			playerState->overlay->UpdateOverlay();
		});
	}
	void Player::StartStream(std::string url)
	{
		Log("Player", "playing url %s", url.c_str());
		if (url != "" && this->state->status == PlayerStatus::Stopped)
		{
			this->state->status = PlayerStatus::Playing;
			this->state->currentURL = url;
			this->StartPlaybackThread();
			this->StartDecodeThread();
		}
	}
	void Player::StartDecodeThread()
	{
		 decode = std::thread([&] {
			AVFormatContext* avFormat;
			AVDictionary* audioOptions = nullptr;
			if (avformat_open_input(&this->state->format, this->state->currentURL.c_str(), nullptr, nullptr) != 0)
			{
				Log("Player", "failed open to url");
				return;
			}
			if (!(avformat_find_stream_info(this->state->format, NULL) >= 0)) { return; }
			for (int i = 0; i < this->state->format->nb_streams; i++)
			{
				if (this->state->format->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
				{
					this->state->videoIdx = i;
					this->state->videoTimeBase = av_q2d(this->state->format->streams[i]->time_base);
				}
				else if (this->state->format->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
				{
					this->state->audioIdx = i;
					this->state->audioTimeBase = av_q2d(this->state->format->streams[i]->time_base);
				}
			}

			Log("Player", "stream starts at %f", this->state->format->streams[this->state->videoIdx]->start_time * this->state->videoTimeBase);

			this->state->audioState->aCtx = this->state->format->streams[this->state->audioIdx]->codec;
			this->state->audioState->audioCodec = avcodec_find_decoder(this->state->audioState->aCtx->codec_id);
			avcodec_open2(this->state->audioState->aCtx, this->state->audioState->audioCodec, &audioOptions);
			av_dump_format(this->state->format, this->state->videoIdx, this->state->currentURL.c_str(), 0);
			this->state->videoClock = (av_gettime() / AV_TIME_BASE);
			this->state->audioState->audioClock = (av_gettime() / AV_TIME_BASE);
			this->state->lastDelay = 40e-3;
			this->InitSDLAudio(this->state->audioState->aCtx->sample_rate,
							   this->state->audioState->aCtx->channels);
			this->Decode();

		});
		decode.detach();
	}
	void Player::StartPlaybackThread()
	{
		play = std::thread([&] {
			Player::SetRefreshTimer(40, this);
		});
		play.detach();
	}
	void Player::InitSDLAudio(int sampleRate, int channels)
	{
		SDL_AudioSpec want, have;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = sampleRate;
		want.format = AUDIO_S16SYS;
		want.channels =	channels;
		want.samples = platformInterface->GetAudioSampleCount();
		want.silence = 0;
		want.callback = Player::SDLAudioCallback;
		want.userdata = this->state;
		this->state->audioDevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
		if (this->state->audioDevice == 0) { Log("Player", "Failed to init SDL audio. %s", SDL_GetError());  }
		else
		{
			Log("Player", "havv.freq = %i", have.freq);
			SDL_PauseAudioDevice(this->state->audioDevice, 0);
		}
	}
	void Player::Decode()
	{
		AVFormatContext* avFormat = this->state->format;
		AVPacket pkt;
		av_init_packet(&pkt);

		TRIGGER_EVENT(EmberStateChange, new EmberStateChangeEventArgs("playing", "notmuted"));

		while(this->CheckPlayerState())
		{
			if (avFormat == nullptr) { break; }

			if (this->state->video.size() > 5 && this->state->audio.size() > 5)
			{
				this->state->bufferSignal->Signal();
			}

			if (av_read_frame(avFormat, &pkt) >= 0)
			{
				if (this->platformInterface->GetQueuedVideo() > 3500) { SDL_Delay(250); }
				if (pkt.stream_index == this->state->videoIdx)
				{
					this->state->video.emplace(Data(pkt.data, pkt.size, pkt.dts * this->state->videoTimeBase));
					av_free_packet(&pkt);
				}
				else if (pkt.stream_index == this->state->audioIdx)
				{
					this->ProcessAudio(&pkt);
					av_free_packet(&pkt);
				}
				else { av_free_packet(&pkt); }
			}
		}
		this->ResetPlayer();
	}
	int Player::ProcessAudio(AVPacket* pkt)
	{
		uint8_t* out = NULL;
		AVFrame* f = av_frame_alloc();
		int samples = 0, bufferSize = 0, lineSize = 0;
		AVCodecContext* fmt = this->state->audioState->aCtx;
		int64_t channels;
		if (fmt->channels == 2) { channels = AV_CH_LAYOUT_STEREO; }
		else if (fmt->channels == 1) { channels = AV_CH_LAYOUT_MONO; }

		SwrContext* convertCtx = swr_alloc_set_opts(nullptr, channels, av_get_sample_fmt("s16"), fmt->sample_rate,
													channels, fmt->sample_fmt, fmt->sample_rate, 0, NULL);
		swr_init(convertCtx);

		if(!avcodec_send_packet(fmt, pkt))
		{
			if(!avcodec_receive_frame(fmt, f))
			{
				samples = av_rescale_rnd(swr_get_delay(convertCtx, fmt->sample_rate) + f->nb_samples,
														fmt->sample_rate, fmt->sample_rate, AV_ROUND_UP);
				av_samples_alloc(&out, NULL, fmt->channels, f->nb_samples, AV_SAMPLE_FMT_S16, 0);
				int ret = swr_convert(convertCtx, &out, samples, (const uint8_t**)f->data, samples);
				bufferSize = av_samples_get_buffer_size(&lineSize, fmt->channels, f->nb_samples, AV_SAMPLE_FMT_S16, 1);
				this->state->audio.emplace(Data(out, bufferSize, pkt->dts * this->state->audioTimeBase));

				int channelsX2 = 2 * fmt->channels;
				av_frame_free(&f);
				swr_free(&convertCtx);
				av_free(&out[0]);
			}
		}
		return 0;
	}
	void Player::Play()
	{
		Data video, audio;
		video = this->state->video.front();

		this->state->overlay->UpdateIntValue("QueuedVideo", this->platformInterface->GetQueuedVideo());
		PlatformManager::Get()->GetPlatformInterface()->DecodeVideoFrame(video.data, video.size);
		video.DeleteData();
		this->state->video.pop();
	}
	void Player::Stop()
	{
		Log("Player", "Stopping stream...");
		bool locked = this->state->stateGuard.try_lock();
		if (locked)
		{
			this->state->status = PlayerStatus::Stopped;
			this->state->stateGuard.unlock();
			this->state->audioState->silence = false;
		}
	}
	double Player::GetAudioClock()
	{
		return 0.0;
	}
	void Player::SetRefreshTimer(int delay, Player* state)
	{
		SDL_AddTimer(delay, Player::RefreshStream, (void*)state);
	}
	void Player::SetBitrate(int bitrate)
	{
		PlatformManager::Get()->GetPlatformInterface()->SetVideoBitrate(bitrate);
	}
	void Player::SetFPS(int fps)
	{
		PlatformManager::Get()->GetPlatformInterface()->SetVideoFPS(fps);
	}
	uint32_t Player::RefreshStream(uint32_t interval, void* opaque)
	{
		Data video, audio;
		Player* playerRef = Player::Get();
		InternalPlayerState* state = Player::Get()->GetPlayerState();
		double delay = 0, actualDelay = 0, avDiff, refreshTime = 0;
		// if (state->video.empty() == false) { Player::Get()->Play(); }
		// Player::SetRefreshTimer(1, playerRef);
		if (state->video.empty() == true) { Player::SetRefreshTimer(1, playerRef); }
		else
		{
			audio = state->audio.front();
			video = state->video.front();
			delay = video.pts - state->lastVideoPts;
			if (delay <= 0 || delay >= 1.0) { delay = state->lastDelay; }
			state->lastVideoPts = video.pts;
			state->lastDelay = delay;

			//TODO: get audio timing some how
			state->videoClock += delay;
			avDiff = state->videoClock - state->audioState->audioClock;

			refreshTime = ((state->audioState->audioClock - state->videoClock) / AV_TIME_BASE) / 1000.0;

			actualDelay = state->videoClock - (av_gettime() / AV_TIME_BASE);
			state->overlay->UpdateDoubleValue("AVClockDiff", avDiff);
			state->overlay->UpdateDoubleValue("AVDelayDiff", actualDelay - avDiff);
			state->overlay->UpdateDoubleValue("FrameTimer", state->videoClock);
			state->overlay->UpdateDoubleValue("VideoActualDelay", actualDelay + avDiff);
			Player::Get()->Play();//actualDelay + avDiff
			Player::SetRefreshTimer(refreshTime, playerRef);
		}

		return 0;
	}
	uint32_t Player::RefreshOverlay(uint32_t interval, void* opaque)
	{
		TRIGGER_EVENT(UpdateOverlay, opaque);
		return interval;
	}
	void Player::SDLAudioCallback(void* userdata, uint8_t* stream, int len)
	{
		Data audio;
		double delay = 0, actualDelay = 0;
		InternalPlayerState* state = (InternalPlayerState*)userdata;
		AudioState* audioState = state->audioState;

		memset(stream, 0, len);
		if (state->audio.empty() == false && state->status == PlayerStatus::Playing)
		{
			audio = state->audio.front();

			delay = audio.pts - state->lastAudioPts;
			//if (delay <= 0 || delay >= 1.0) { delay = audioState->lastDelay; }
			state->lastAudioPts = audio.pts;
			audioState->lastDelay = delay;
			audioState->audioClock += delay;
			actualDelay = audioState->audioClock - (av_gettime() / AV_TIME_BASE);

			if (state->audioState->silence == false) { memcpy(stream, audio.data, len); }

			state->overlay->UpdateDoubleValue("AudioDelay", actualDelay);
			state->overlay->UpdateDoubleValue("AudioClock", audioState->audioClock);
			audio.DeleteData();
			state->audio.pop();
		}
	}
	bool Player::CheckPlayerState()
	{
		bool isPlaying = true;
		bool locked = this->state->stateGuard.try_lock();
		if (locked)
		{
			if (this->state->status == PlayerStatus::Stopped) { isPlaying = false; }
			this->state->stateGuard.unlock();
		}
		return isPlaying;
	}
	void Player::SigTermHandler(int signum)
	{
		exit(0);
	}
	void Player::Mute()
	{
		if (this->state->audioState->silence)
		{
			this->state->audioState->silence = false;
			TRIGGER_EVENT(EmberStateChange, new EmberStateChangeEventArgs("playing", "notmuted"));
		}
		else
		{
			this->state->audioState->silence = true;
			TRIGGER_EVENT(EmberStateChange, new EmberStateChangeEventArgs("playing", "muted"));
		}
	}
	void Player::ResetPlayer()
	{
		platformInterface->DecoderReset();
		Data d;
		if (this->state->video.size() > 0)
		{
			while (!this->state->video.empty())
			{
				d = this->state->video.front();
				d.DeleteData();
				this->state->video.pop();
			}
		}
		if (this->state->audio.size() > 0)
		{
			while (!this->state->audio.empty())
			{
				d = this->state->audio.front();
				d.DeleteData();
				this->state->audio.pop();
			}
		}
		this->state->format = nullptr;
		this->state->videoClock = 0;
		this->state->lastAudioPts = 0;
		this->state->audioState->lastDelay = 0;
		this->state->audioState->audioClock = 0;
		avformat_close_input(&this->state->format);
		avcodec_close(this->state->audioState->aCtx);
		SDL_CloseAudioDevice(this->state->audioDevice);
	}
	void Player::SetEmberEventHandlers()
	{
		HANDLE_EVENT(EmberStartStream, true, "PlayerApp", HANDLER {
			if (args != nullptr)
			{
				EmberStreamEventArgs *eventArgs = (EmberStreamEventArgs*)args;
				this->StartStream(eventArgs->GetValue());
			}
		});
		HANDLE_EVENT(EmberStopStream, true, "PlayerApp", HANDLER {
			this->Stop();
		});
		HANDLE_EVENT(EmberMuteStream, true, "PlayerApp", HANDLER {
			this->Mute();
		});
		HANDLE_EVENT(EmberUnmuteStream, true, "PlayerApp", HANDLER {
			this->Mute();
		})
	}
}}