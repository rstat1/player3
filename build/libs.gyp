# Copyright (c) 2012 The Incredibly Big Red Robot.
#
# Use of this source code is governed by the "Do what I say, not what I do" license that can't be
# found in the srcd LICENSE file.

{
	'targets':
	[
		{
			'target_name': 'base',
			'msvs_guid': '2BAEC9E5-8F23-47C6-93E9-C3B328B3DE65',
			'include_dirs':
			[
				'../src/',
				'../external',
			],
			'defines': [
				'BOOST_DATE_TIME_NO_LIB'
			],
			'conditions': [
				['OS=="linux"', {
					'type': 'static_library',
					'cflags_cc': [
						'-std=c++11',
						'-frtti',
						'-fPIC', '-g3', '-O -g'
		            ],
					'defines': [
						'LINUX',
					],
					'ldflags': [ '-rdynamic' ],
					'link_settings': {
						'libraries': [
							'-lpthread',
							'-lstdc++'
						],
					},
					'sources': [
						'../src/base/platform/linux/memtrack.cpp',
						#'../src/base/platform/linux/ipc/PlatformPipePosix.cpp',
						'../src/base/platform/linux/threading/PlatformThreadPosix.cpp',
						'../src/base/platform/linux/threading/ConditionVariablePosix.cpp',
						#'../src/base/platform/linux/threading/ThreadLocalStoragePosix.cpp',
						'../src/base/platform/linux/dispatcher/MessagePumpEPoll.cpp',
						'../src/base/platform/linux/dispatcher/MessagePumpEPoll.h',
						'../src/base/platform/linux/dispatcher/DispatcherTypesLinux.h',
						'../src/base/platform/linux/memtrack.h',
					],
				}],
				['skylight_arch=="x86_64"', {
					'conditions': [
						['OS=="linux"', {
							'link_settings': {
								'libraries': [
									'<(buildPath)/external/libs/linux/x86_64/libboost_system.a',
									'<(buildPath)/external/libs/linux/x86_64/libboost_filesystem.a'
								],
							},
						}],
					]
				}],
				['skylight_arch=="armv7l"', {
					'link_settings': {
						'libraries': [
							'<(buildPath)/external/libs/linux/armv7l/libboost_system.a',
							'<(buildPath)/external/libs/linux/armv7l/libboost_filesystem.a'
						],
					},
				}],
			],
			'sources': [
				'../src/base/Utils.cpp',
				'../src/base/logging.cpp',
				'../src/base/threading/Thread.cpp',
				'../src/base/threading/TaskRunner.cpp',
				'../src/base/threading/dispatcher/Dispatcher.cpp',
				#'../src/base/memory/GarbageCollected.cpp',
				#'../src/base/memory/GarbageCollected.h',
				'../src/base/common.h',
				'../src/base/Utils.h',
				'../src/base/logging.h',
				'../src/base/base_exports.h',
				'../src/base/threading/common/ConditionVariable.h',
				'../src/base/threading/common/Thread.h',
				'../src/base/threading/common/thread_types.h',
				'../src/base/threading/dispatcher/MessagePump.h',
				'../src/base/threading/dispatcher/Dispatcher.h',
				'../src/base/threading/dispatcher/DispatcherMessagePump.h',
				'../src/base/threading/dispatcher/DispatcherTypes.h',
				#'../src/base/threading/tls/ThreadLocalStorage.h',
				#'../src/base/threading/tls/ThreadLocalTypes.h',
				'../src/base/threading/common/PlatformThread.h',
				'../src/base/threading/common/TaskRunner.h',
			],
		},
		{
			'target_name': 'external',
			'msvs_guid': '2BAEC9E5-8F23-47C6-93E9-C3B328B3DE65',
			'include_dirs': [
				#'../src/',
				#'../external/nanovg/',
				#'../external/jsoncpp/',
			],
			'sources': [
				#'../external/nanovg/nanovg.c',
				#'../external/jsoncpp/jsoncpp.cpp',
				#'../external/jsoncpp/json/json.h',
				#'../external/nanovg/stb_image.h',
				#'../external/nanovg/stb_truetype.h',
				#'../external/nanovg/nanovg.h',
				#'../external/nanovg/nanovg_gl.h',
				#'../external/nanovg/nanovg_gl_utils.h',
				#'../external/nanovg/fontstash.h',
			],
			'conditions': [
				['OS=="win"', {
					'type': "static_library"
				}],
				['OS=="linux"', {
					'type': "shared_library",
					'cflags': [
						'-fPIC',
					],
					'link_settings': {
						'libraries': [
							'-lpthread',
							'-lstdc++'
						],
					},
				}],
				['with_profiler==1', {
				    'cflags': [
				        '-std=gnu11'
				    ],
					'sources':[
						#'../external/remotery/Remotery.c',
						#'../external/remotery/Remotery.h',
					],
				}],
				['skylight_arch=="x86_64"', {
					'defines': [
						'NANOVG_GLEW'
					],
					'conditions': [
						['OS=="win"', {
							'link_settings': {
								'libraries': [
									#'../external/libs/windows/x86_64/glew32.lib',
								],
							},
						}],
						['OS=="linux"', {
							'link_settings': {
								'libraries': [
									'-lGL',
									'-lGLU',
									'-lm',
									'<(buildPath)/external/libs/linux/x86_64/libGLEW.a',
								],
							},
						}],
					],
				}],
				['skylight_arch=="armv7l"', {
					'conditions': [
						['OS=="linux"', {
							'link_settings': {
								'libraries': [
								    '-lEGL',
									'-lGLESv2',
									'-lm',
								],
							},
						}],
					],
				}]
			]
		}
	],
}
