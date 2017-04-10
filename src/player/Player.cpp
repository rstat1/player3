/*
* Copyright (c) 2017 The Incredibly Big Red Robot
*
* Use of this source code is governed by a "BSD-style" license that can be
* found in the included LICENSE file.
*/

#include <thread>
#include <SDL.h>
#include <player/Player.h>

extern "C"
{
	#include <libavutil/avutil.h>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
};

namespace streamlink { namespace player
{
	Player::Player()
	{
		av_register_all();
		avcodec_register_all();
		avformat_network_init();
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
		this->state = new InternalPlayerState();
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
			AVFormatContext* avFormat = nullptr;
			AVPacket pkt;
			if (avformat_open_input(&avFormat, this->state->currentURL.c_str(), NULL, NULL) == 0)
			{
				if (avformat_find_stream_info(avFormat, NULL) >= 0)
				{
					for (int i = 0; i < avFormat->nb_streams; i++)
					{
						if (avFormat->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) { this->state->videoIdx = i; }
						else if (avFormat->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) { this->state->audioIdx = i; }
					}
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
			}
			else { Log("Player", "failed to open url"); }
		});
	}
	void Player::Play()
	{
		while(this->state->status == PlayerStatus::Playing)
		{

		}
	}
}}