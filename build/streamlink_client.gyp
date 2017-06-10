# Copyright (c) 2016 The Incredibly Big Red Robot.
#
# Use of this source code is governed by the "Do what I say, not what I do" license that can't be
# found in the srcd LICENSE file.

{
	'targets':
	[
		{
			'target_name': 'player3',
			'type': "executable",
			'msvs_guid': '2BAEC9E5-8F23-47C6-93E9-C3B328B3DE65',
			'include_dirs':
			[
				'../src/',
				'../external/',
			],
			'dependencies': [
				'streamlink_client.gyp:player3lib',
				'libs.gyp:base'
			],
			'conditions': [
				['OS=="linux"', {
					'cflags': [
						'-std=c++11',
						'-frtti',
						'-fPIC', '-g3', '-O -g',
					],
					'dependencies': [
						'../external/breakpad/breakpad.gyp:breakpad_client',
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
							'-lstdc++'
						],
					},
				}],
			],
		},
		{
			'target_name': 'player3lib',
			'msvs_guid': '2BAEC9E5-8F23-47C6-93E9-C3B328B3DE65',
			'include_dirs':
			[
				'../src/',
				'../external/',
				"../external/ffmpeg",
				'../external/cpr/include/',
				'../external/sdl/include/',
				'../external/seasocks/src/main/c/',
			],
			'defines': [
			],
			'dependencies': [
				'libs.gyp:base',
				'libs.gyp:seasocks',
				'libs.gyp:cpr',
				'libs.gyp:external'
			],
			'sources': [
				'../src/PlayerApp.cpp',
				'../src/player/Player.cpp',
				'../src/player/infooverlay/InfoOverlay.cpp',
				'../src/ui/UIServer.cpp',
				'../src/ui/packaging/Archive.cpp',
				'../src/ui/packaging/AsarResponse.cpp',
				'../src/ui/handlers/WebPageHandler.cpp',
				'../src/ui/handlers/WebSocketHandler.cpp',
				'../src/ui/packaging/File.cpp',
				'../src/ui/packaging/File.h',
				'../src/ui/UIServer.h',
				'../src/player/Player.h',
				'../src/player/infooverlay/InfoOverlay.h',
				'../src/PlayerApp.h',
				'../src/player/VideoPlayerBase.h',
				'../src/platform/Platfroms.h',
				'../src/platform/PlatformInterface.h',
				'../src/ui/packaging/Archive.h',
				'../src/ui/packaging/AsarResponse.h',
				'../src/ui/handlers/WebPageHandler.h',
				'../src/ui/handlers/WebSocketHandler.h'
			],
			'conditions': [
				['OS=="linux"', {
					'type': 'static_library',
					'defines': [
						'LINUX',
						'__STDC_CONSTANT_MACROS'
					],
					'dependencies': [],
					'cflags_cc': [
						'-std=c++11',
						'-frtti',
						'-fPIC', '-g3', '-g',
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
							'-lSDL2_ttf',
						],
					},
					'sources': []
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
						'../src/platform/steamlink/SteamLinkPlatform.cpp',
						'../src/platform/steamlink/SteamLinkPlatform.h',
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
						'../src/platform/desktop/DesktopPlatform.cpp',
						'../src/platform/desktop/DesktopPlatform.h',
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