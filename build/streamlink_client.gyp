# Copyright (c) 2016 The Incredibly Big Red Robot.
#
# Use of this source code is governed by the "Do what I say, not what I do" license that can't be
# found in the srcd LICENSE file.

{
	'targets':
	[
		{
			'target_name': 'streamlink_player',
			'type': "executable",
			'msvs_guid': '2BAEC9E5-8F23-47C6-93E9-C3B328B3DE65',
			'include_dirs':
			[
				'../src/',
				'../external/',
			],
			'dependencies': [
				'streamlink_client.gyp:streamlink',
				'libs.gyp:base'
			],
			'conditions': [
				['OS=="linux"', {
					'cflags': [
						'-std=c++11',
						'-frtti',
						'-fPIC', '-g3', '-O -g',
					],
					'defines': [
						'LINUX',
					],
					'include_dirs': [
						'<(SDLIncludeDir)',
						'<(SLIncludesDir)',
						'../external/breakpad/src/',
					],
					'sources': [
						'../src/main.cpp',
					],
					'link_settings': {
						'libraries': [

						],
					},
				}],
			],
		},
		{
			'target_name': 'streamlink',
			'msvs_guid': '2BAEC9E5-8F23-47C6-93E9-C3B328B3DE65',
			'include_dirs':
			[
				'../src/',
				'../external',
				'../external/sdl/include',
				"../external/ffmpeg",
			],
			'defines': [
			],
			'dependencies': [
				'libs.gyp:base',
			],
			'sources': [
				'../src/player/Player.cpp',
				'../src/player/Player.h',
				#'../src/app/native/StreamLinkApp.cpp',
				#'../src/app/native/mono/MonoHost.cpp',
				#'../src/app/native/mono/MonoNativeProxy.cpp',
				#'../src/app/native/mono/InputReceiverNative.cpp',
				#'../src/app/native/video/demux/ClipProcessor.cpp',
				#'../src/app/native/video/VideoProcessorThread.cpp',
				#'../src/app/native/video/VideoProcessorThread.h',
				#'../src/app/native/video/VideoPlayer.cpp',
				#'../src/app/native/video/VideoPlayer.h',
				#'../src/app/native/decoders/StreamDecoder.h',
				#'../src/app/native/decoders/StreamDecoders.h',
				#'../src/app/native/video/ClipProcessor.h',
				#'../src/app/native/mono/InputReceiverNative.h',
				#'../src/app/native/mono/MonoNativeProxy.h',
				#'../src/streamlink_exports.h',
				#'../src/app/StreamLinkApp.h',
				#'../src/app/native/mono/MonoHost.h',
			],
			'conditions': [
				['OS=="linux"', {
					'type': 'static_library',
					'defines': [
						'LINUX',
						'__STDC_CONSTANT_MACROS'
					],
					'dependencies': [
					#	'../external/breakpad/breakpad.gyp:breakpad_client',
					],
					'cflags_cc': [
						'-std=c++11',
						'-frtti',
						'-fPIC', '-g3', '-O -g',
						'-fno-stack-protector',
						'-Wno-deprecated-declarations',

					],
					'include_dirs': [
						'../external/breakpad/src/',
						'<(SLIncludesDir)',
					],
					'link_settings': {
						'libraries': [
							'-pthread',
							'-lSDL2',
						],
					},
					'sources': [
					#	'../src/ui/graphics/NativeGraphicsAPILinux.cpp',
					#	'../src/base/platform/SharedThreadState.h'
					]
				}],
				['skylight_arch=="armv7l"', {
					'link_settings': {
						'libraries': [
							'-lEGL',
							'-lGLESv2',
							'-lrt',
							'-ldl'
						],
					},
					'sources': [
						#'../src/app/native/decoders/StreamDecoderSteamLink.cpp',
						#'../src/app/native/decoders/StreamDecoderSteamLink.h',
					],
					'conditions': [
						['OS=="linux"', {
							'link_settings': {
								'libraries': [
								#	'../external/libs/linux/armv7l/libmonosgen-2.0.a',
									'../external/libs/linux/armv7l/libswresample.a',
									'../external/libs/linux/armv7l/libavformat.a',
									'../external/libs/linux/armv7l/libavcodec.a',
									'../external/libs/linux/armv7l/libavutil.a',
									'-lSLVideo',
									'-lSLAudio',
									'-lssl',
									'-lcrypto'
								],
							},
						}]
					],
				}],
				['skylight_arch=="x86_64"', {
					'sources': [
						#'../src/app/native/decoders/StreamDecoderNull.cpp',
						#'../src/app/native/decoders/StreamDecoderNull.h',
					],
					'conditions': [
						['OS=="linux"', {
							'include_dirs': [
								'/usr/include/gl/',
							],
							'link_settings': {
								'libraries': [
									#'../external/libs/linux/x86_64/libmonosgen-2.0.a',
									'-ldl',
									'-lrt',
									'-lm',
									'../external/libs/linux/x86_64/libavformat.a',
									'../external/libs/linux/x86_64/libavcodec.a',
									'../external/libs/linux/x86_64/libavutil.a',
									'../external/libs/linux/x86_64/libswresample.a',
									'-lssl',
									'-lcrypto',
								],
							},
						}]
					],
				}]
			]
		},
	],
}