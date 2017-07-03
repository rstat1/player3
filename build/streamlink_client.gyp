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
				'libs.gyp:base',
			],
			'conditions': [
				['OS=="linux"', {
					'cflags': [
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
				['skylight_arch=="armv7l"', {
					'cflags_cc': [
						'-std=c++1y'
					],
				}],
				['skylight_arch=="x86_64"', {
					'cflags_cc': [
						'-std=c++14'
					],
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
				'../external/duktape/',
				"../external/ffmpeg",
				'../external/cpr/include/',
				'../external/sdl/include/',
				'../external/seasocks/src/main/c/',
			],
			'defines': [
			],
			'dependencies': [
				'libs.gyp:base',
				'libs.gyp:uWS',
				'libs.gyp:cpr',
				'libs.gyp:external',
			],
			'sources': [
				'../src/PlayerApp.cpp',
				'../src/player/Player.cpp',
				'../src/player/infooverlay/InfoOverlay.cpp',
				'../src/ui/web/UIServer.cpp',
				'../src/ui/web/packaging/Archive.cpp',
				'../src/ui/web/packaging/AsarResponse.cpp',
				'../src/ui/web/handlers/WebPageHandler.cpp',
				'../src/ui/web/handlers/WebSocketHandler.cpp',
				'../src/ui/web/packaging/File.cpp',
				'../src/ui/native/rendering/NanoVGRenderer.cpp',
				'../src/ui/native/NativeUIHost.cpp',
				'../src/platform/PlatformManager.cpp',
				'../src/ui/native/LayoutManager.cpp',
				'../src/ui/native/elements/LabelElement.cpp',
				'../src/player/chat/ChatService.cpp',
				'../src/player/chat/ChatServiceThread.cpp',
				'../src/ui/web/UIServer.h',
				'../src/player/Player.h',
				'../src/player/infooverlay/InfoOverlay.h',
				'../src/PlayerApp.h',
				'../src/player/VideoPlayerBase.h',
				'../src/platform/Platfroms.h',
				'../src/platform/PlatformInterface.h',
				'../src/ui/web/packaging/Archive.h',
				'../src/ui/web/handlers/WebPageHandler.h',
				'../src/ui/web/handlers/WebSocketHandler.h'
				'../src/ui/native/rendering/NanoVGDefines.h',
				'../src/ui/native/rendering/NanoVGRenderer.h',
				'../src/ui/native/rendering/RendererTypes.h',
				'../src/ui/web/packaging/File.h',
				'../src/BuildInfo.h',
				'../src/ui/native/NativeUIHost.h',
				'../src/platform/PlatformManager.h',
				'../src/ui/native/LayoutManager.h',
				'../src/ui/native/ElementBase.h',
				'../src/ui/native/elements/LabelElement.h',
				'../src/player/chat/ChatService.h',
				'../src/player/chat/ChatServiceThread.h',
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
							'cflags_cc': [
								'-std=c++1y',
							],
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
							'cflags_cc': [
								'-std=c++14',
							],
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