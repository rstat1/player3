/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <SDL.h>
#include <chrono>
#include <thread>
#include <signal.h>
#include <player/Player.h>
#include <base/platform/linux/memtrack.h>

#include <iostream>

using namespace std;
using namespace base::platform;

#define AV_SYNC_THRESHOLD 0.01

namespace player3 { namespace player
{
	double Player::lastMemoryUse;
	PlatformInterface* Player::platformInterface;

	Player::Player()
	{
		Log("Player", "player3 player init");
		av_log_set_level(AV_LOG_VERBOSE);
		av_register_all();
		avcodec_register_all();
		avformat_network_init();
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO);

		VIDEO_DECODE_INIT

		this->state = new InternalPlayerState();
		this->state->frameTimer = 0;
		this->state->lastVideoPts = 0;
		this->state->lastAudioPts = 0;
		this->state->audioDecodeState = new AudioState();
		this->state->audioDecodeState->audioClock = 0;
		this->state->bufferSignal = new ConditionVariable();

		this->InitOverlay();

		std::thread test([&] {
			Overlay* o = this->state->overlay->UpdateOverlay();
			if (o != nullptr)
			{
				platformInterface->CreateOverlay(o->surfaceW, o->surfaceH);
				platformInterface->ShowOverlay(o->overlay->pixels, o->pitch);
				SDL_AddTimer(750, Player::RefreshOverlay, this->state);
			}
		});
		test.detach();
#if defined(OS_LINUX) && !defined(OS_STEAMLINK)
		signal(SIGTERM, Player::SigTermHandler);
#endif
	}
	void Player::InitOverlay()
	{
		this->state->overlay = new InfoOverlay();
		this->state->overlay->InitOverlay();
		this->state->overlay->AddDoubleValue("MemPeak (in MB)", 0.0);
		this->state->overlay->AddDoubleValue("MemCurrent (in MB)", MemTrack::GetCurrentMemoryUse());
		this->state->overlay->AddIntValue("AudioCallbackTime (in ms)", 0);
		this->state->overlay->AddDoubleValue("AudioClock", 0);
		this->state->overlay->AddDoubleValue("VideoDelay", 0);
	}
	void Player::StartStream(std::string url)
	{
		this->state->currentURL = url;
		this->StartPlaybackThread();
		this->StartDecodeThread();
	}
	void Player::StartDecodeThread()
	{
		 std::thread decode([&] {
			AVFormatContext* avFormat;
			AVDictionary* audioOptions = nullptr;
			cout << av_gettime() / 1000000.0 << endl;
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

			this->state->audioDecodeState->aCtx = this->state->format->streams[this->state->audioIdx]->codec;
			this->state->audioDecodeState->audioCodec = avcodec_find_decoder(this->state->audioDecodeState->aCtx->codec_id);
			avcodec_open2(this->state->audioDecodeState->aCtx, this->state->audioDecodeState->audioCodec, &audioOptions);
			av_dump_format(this->state->format, this->state->videoIdx, this->state->currentURL.c_str(), 0);
			this->state->frameTimer = av_gettime() / 1000000.0;
			this->InitSDLAudio(this->state->audioDecodeState->aCtx->sample_rate);
			this->Decode();
		});
		decode.detach();
	}
	void Player::StartPlaybackThread()
	{
		std::thread play([&] {
			this->Play();
		});
		play.detach();
	}
	void Player::InitSDLAudio(int sampleRate)
	{
		SDL_AudioSpec want, have;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = sampleRate;
		want.format = AUDIO_S16SYS;
		want.channels =	2;
		want.samples = 1024; //platformInterface->GetAudioSampleCount();
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

		while(this->state->status == PlayerStatus::Playing)
		{
			if (this->state->video.size() > 5 && this->state->audio.size() > 5)
			{
				this->state->bufferSignal->Signal();
				// SDL_Delay(10);
			}
			if (av_read_frame(avFormat, &pkt) >= 0)
			{
				if (pkt.stream_index == this->state->videoIdx)
				{
					this->state->video.emplace(Data(pkt.data, pkt.size, pkt.pts * this->state->videoTimeBase));
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
	}
	int Player::ProcessAudio(AVPacket* pkt)
	{
		uint8_t* out = NULL;
		AVFrame* f = av_frame_alloc();
		int samples = 0, bufferSize = 0, lineSize = 0;
		AVCodecContext* fmt = this->state->audioDecodeState->aCtx;

		SwrContext* convertCtx = swr_alloc_set_opts(nullptr, AV_CH_LAYOUT_STEREO, av_get_sample_fmt("s16"), fmt->sample_rate,
										AV_CH_LAYOUT_STEREO, fmt->sample_fmt, fmt->sample_rate, 0, NULL);
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
				this->state->audio.emplace(Data(out, bufferSize, pkt->pts * this->state->audioTimeBase));

				int channelsX2 = 2 * fmt->channels;
				this->state->audioDecodeState->audioClock += (double)lineSize / (double)(channelsX2 * fmt->sample_rate);
				this->state->overlay->UpdateDoubleValue("AudioClock", this->state->audioDecodeState->audioClock);

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
		double delay = 0, actualDelay = 0, sync_threshold;
		while(this->state->status == PlayerStatus::Playing)
		{
			if (this->state->video.empty() == false)
			{
				video = this->state->video.front();
				delay = video.pts - this->state->lastVideoPts;
				if (delay <= 0 || delay >= 1.0) { delay = this->state->lastDelay; }
				this->state->lastVideoPts = video.pts;
				this->state->lastDelay = delay;

				sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
				

				this->state->frameTimer += delay;
				actualDelay = (double)(av_gettime() / 1000000.0) - this->state->frameTimer;

				this->state->overlay->UpdateDoubleValue("VideoDelay", delay);

				if (!platformInterface->DecodeVideoFrame(video.data, video.size)) { Log("play-thread", "Something failed in decode"); }
				video.DeleteData();
				this->state->video.pop();
			}
			else { this->state->bufferSignal->Wait(); }
		}
		Log("Player", "stopped..");
	}
	void Player::Stop()
	{
		this->state->status = PlayerStatus::Stopped;
		platformInterface->DecoderShutdown();
	}
	double Player::GetAudioClock()
	{
		return 0.0;
	}
	uint32_t Player::RefreshStream(uint32_t interval, void* opaque)
	{
		return interval;
	}
	uint32_t Player::RefreshOverlay(uint32_t interval, void* opaque)
	{
		InternalPlayerState* playerState = (InternalPlayerState*)opaque;
		double currentUse = MemTrack::GetCurrentMemoryUse();
		if (currentUse != lastMemoryUse)
		{
			playerState->overlay->UpdateDoubleValue("MemPeak (in MB)", MemTrack::GetPeakMemoryUse());
			playerState->overlay->UpdateDoubleValue("MemCurrent (in MB)", MemTrack::GetCurrentMemoryUse());
			lastMemoryUse = currentUse;
		}
		chrono::time_point<std::chrono::steady_clock> cbTime = std::chrono::steady_clock::now();
		int cbTimeMs = chrono::duration_cast<std::chrono::milliseconds>(cbTime.time_since_epoch()).count();

		playerState->overlay->UpdateIntValue("AudioCallbackTime (in ms)", cbTimeMs - playerState->audioCBTime);

		Overlay* overlay = playerState->overlay->UpdateOverlay();
		Player::platformInterface->ShowOverlay(overlay->overlay->pixels, overlay->pitch);
		return interval;
	}
	void Player::SDLAudioCallback(void* userdata, uint8_t* stream, int len)
	{
		Data audio;
		InternalPlayerState* state = (InternalPlayerState*)userdata;
		AudioState* audioState = state->audioDecodeState;

		chrono::time_point<std::chrono::steady_clock> cbTime = std::chrono::steady_clock::now();
		state->audioCBTime = chrono::duration_cast<std::chrono::milliseconds>(cbTime.time_since_epoch()).count();

		memset(stream, 0, len);
		if (state->audio.empty() == false)
		{
			audio = state->audio.front();
			//if (audio.pts != 0) { SDL_Delay }
			memcpy(stream, audio.data, len);

			audio.DeleteData();
			state->audio.pop();
		}
	}
	void Player::SigTermHandler(int signum)
	{
		exit(0);
	}
}}