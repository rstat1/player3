{
	'variables': {
		'renderer_impl%': 'NanoVG',
		'with_profiler%': 1,
	},
	'target_defaults': {
		'conditions': [
			['skylight_arch=="armv7l"', {
				'default_configuration': 'Debug-armv7l',
				'configurations': {
					'Debug-armv7l': {
					},
				}
			}],
			['with_profiler==1', {
				'defines': [
					'ENABLE_PROFILER'
				]
			}],
			['skylight_arch=="x86_64"', {
				'default_configuration': 'Debug_x64',
				'msvs_target_platform': 'x64',
				'configurations': {
					'Debug_x64': {
						'msvs_configuration_platform': 'x64',
						'msvs_settings': {
							'VCCLCompilerTool': {
								'DebugInformationFormat': '4', # editAndContiue (/ZI)
								'ProgramDataBaseFileName': '$(OutDir)\\$(ProjectName).pdb',
								'Optimization': '0',           # optimizeDisabled (/Od)
								'PreprocessorDefinitions': ['_DEBUG'],
								'RuntimeLibrary': '3',         # rtMultiThreadedDebugDLL (/MDd)
								'ExceptionHandling': '1',
								'RuntimeTypeInfo': 'false',      # /GR-
								'WarningLevel': '1',             # level3 (/W3)
							},
							'VCLinkerTool': {
								'GenerateDebugInformation': 'true', # /DEBUG
								'LinkIncremental': '2',             # /INCREMENTAL

							},
						},
					},
					'Release_x64': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'DebugInformationFormat': '3',      # programDatabase (/Zi)
								'ProgramDataBaseFileName': '$(OutDir)\\$(ProjectName).pdb',
								'Optimization': '3',                # full (/Ox)
								'WholeProgramOptimization': 'true', #/GL
								'FavorSizeOrSpeed': '1',            # speed (/Ot)
								'PreprocessorDefinitions': ['NDEBUG'],
								'RuntimeLibrary': '2',              # rtMultiThreadedDLL (/MD)
								'ExceptionHandling': '0',
								'EnableEnhancedInstructionSet': '2',# /arch:SSE2
								'RuntimeTypeInfo': 'false',         # /GR-
								'WarningLevel': '1',                # level1 (/W3)
							},
							'VCLinkerTool': {
								'GenerateDebugInformation': 'true', # /DEBUG
								'LinkTimeCodeGeneration': '1',      # useLinkTimeCodeGeneration /LTCG
							},
							'VCLibrarianTool': {
								'LinkTimeCodeGeneration': 'true',   # useLinkTimeCodeGeneration /LTCG
							},
						},
					},
				}
			}],
		],
	}
}
