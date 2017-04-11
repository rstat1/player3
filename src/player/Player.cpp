/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <SDL.h>
#include <player/Player.h>

namespace streamlink { namespace player
{
	Player::Player()
	{
		av_log_set_level(AV_LOG_VERBOSE);

		av_register_all();
		avcodec_register_all();
		avformat_network_init();
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
		this->state = new InternalPlayerState();
		this->state->audioDecodeState = new AudioState();
		this->state->bufferSignal = new ConditionVariable();
	}
	void Player::StartStream(std::string url)
	{
		this->state->currentURL = url;
		this->StartDecodeThread();
		this->Play();
	}
	void Player::StartDecodeThread()
	{
		std::thread decode([&] {
			if (avformat_open_input(&this->state->format, this->state->currentURL.c_str(), NULL, NULL) == 0)
			{
				this->InitDecode();
			}
			else { Log("Player", "failed to open url"); }
			this->Decode();
		});
		decode.detach();
	}
	void Player::InitSDLAudio(int sampleRate)
	{
		SDL_AudioSpec want, have;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = sampleRate;
		want.format = AUDIO_S16SYS;
		want.channels =	2;
		want.samples = 1024;
		this->state->audioDevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
		if (this->state->audioDevice == 0) { Log("Player", "Failed to init SDL audio. %s", SDL_GetError());  }
		else { SDL_PauseAudioDevice(this->state->audioDevice, 0); }
	}
	void Player::InitDecode()
	{
		AVDictionary* audioOptions = nullptr;
		AVFormatContext* avFormat = this->state->format;

		if (avformat_find_stream_info(avFormat, NULL) >= 0)
		{
			for (int i = 0; i < avFormat->nb_streams; i++)
			{
				if (avFormat->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) { this->state->videoIdx = i; }
				else if (avFormat->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
				{
					this->state->audioIdx = i;
					this->state->audioDecodeState->aCtx = avFormat->streams[i]->codec;
					this->state->audioDecodeState->audioCodec = avcodec_find_decoder(this->state->audioDecodeState->aCtx->codec_id);
					avcodec_open2(this->state->audioDecodeState->aCtx, this->state->audioDecodeState->audioCodec, &audioOptions);
					this->InitSDLAudio(avFormat->streams[this->state->audioIdx]->codec->sample_rate);
				}
			}
		}
	}
	void Player::Play()
	{
		while(this->state->status == PlayerStatus::Playing)
		{

		}
	}
	void Player::Decode()
	{
		AVPacket pkt;
		AVFormatContext* avFormat = this->state->format;
		while(this->state->status == PlayerStatus::Playing)
		{
			if (this->state->video.size() > 10 || this->state->audio.size() > 10) { SDL_Delay(10); }
			if (av_read_frame(avFormat, &pkt) >= 0)
			{
				if (pkt.stream_index == this->state->videoIdx)
				{
					this->state->video.emplace_back(Data(pkt.data, pkt.size));
					av_free_packet(&pkt);
				}
				else if (pkt.stream_index == this->state->audioIdx)
				{
					//TODO: audio decode;
					av_free_packet(&pkt);
				}
				else { av_free_packet(&pkt); }
			}
		}
	}
}}