# Copyright (c) 2016 The Incredibly Big Red Robot.
#
# Use of this source code is governed by the "Do what I say, not what I do" license that can't be
# found in the included LICENSE file.

{
	'targets':
	[
		{
			'target_name': 'everything',
			'type': 'none',
			'dependencies':	[
				'streamlink_client.gyp:streamlink',
				'streamlink_client.gyp:streamlink_player',
			],
			'conditions': [
				['OS=="linux"', {
					'defines': [
						'LINUX'
					],
				}],
			],
		}
	],
}
