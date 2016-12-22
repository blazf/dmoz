{
    'variables': {
        'LIN_ALG_BLAS%': 'NBLAS',
        'LIN_ALG_LAPACKE%': 'NLAPACKE',
        'LIN_ALG_EIGEN%': 'NEIGEN',
        'LIN_ALG_INCLUDE%': 'qminer/src/glib/base/',
        'LIN_EIGEN_INCLUDE%': 'qminer/src/third_party/eigen/',
        #full path to lapack or openblas libraries
        'LIN_ALG_LIB%': '',
        #64 bit indexing for BLAS
        'INDEX_64%': 'NINDEX_64',
        'INTEL%': 'NINTEL'
    },
    'target_defaults': {
        'default_configuration': 'Release',
        'configurations': {
            'Debug': {
                'defines': [
                    'DEBUG',
                ],
            },
            'Release': {
                'defines': [
                    'NDEBUG'
                ],
            }
        },
        'defines': [
            '<(LIN_ALG_BLAS)',
            '<(LIN_ALG_LAPACKE)',
            '<(LIN_ALG_EIGEN)',
            '<(INDEX_64)',
            '<(INTEL)'
        ],
        # hack for setting xcode settings based on example from
        # http://src.chromium.org/svn/trunk/o3d/build/common.gypi
        'target_conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                    'MACOSX_DEPLOYMENT_TARGET': '10.7',
                    'GCC_ENABLE_CPP_RTTI': 'YES',
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'OTHER_CFLAGS': [ '-std=c++11', '-stdlib=libc++' ]
                },
            }],
        ],
        'conditions': [
            # operating system specific parameters
            ['OS == "linux"', {
                "link_settings": {
                    "libraries": [ '-lrt', '-fopenmp', '<(LIN_ALG_LIB)' ],
                },
                # GCC flags
                'cflags_cc!': [ '-fno-rtti', '-fno-exceptions' ],
                'cflags_cc': [ '-std=c++0x', '-frtti', '-fexceptions' ],
                'cflags': [ '-Wno-deprecated-declarations', '-fopenmp' ],
                # additional flags for ARM
                'conditions': [
                    ['target_arch == "arm"', {
                        "link_settings": {
                            "ldflags": [ '-Wl,--allow-multiple-definition' ]
                        },
                        'cflags!': [ ], # add -g if low on memory and gcc fails in debug mode
                        'cflags': [ '-fsigned-char' ], # add -g if you need symbols in release mode
                        'defines': [ "ARM" ]
                    }]
                ]
            }],
            ['OS == "win"', {
                # 'msbuild_toolset': 'v120', # works with v120 (--msvs_version=2013) and v140 (--msvs_version=2015)
                'msvs_settings': {
                    'VCCLCompilerTool': {
                        #'RuntimeTypeInfo': 'true',      # /GR  : this should work but doesn't get picked up
                        #'ExceptionHandling': 1,         # /EHsc: this should work but doesn't get picked up
                        'OpenMP': 'true',
                        "AdditionalOptions": [ "/EHsc /GR" ] # release mode displays D9025 warnings, which is a known issue https://github.com/nodejs/node-gyp/issues/335
                    },
                    'VCLinkerTool': {
                        'SubSystem' : 1, # Console
                        'AdditionalOptions': ['<(LIN_ALG_LIB)']
                    },
                },
            }],
            ['OS == "mac"', {
                "default_configuration": "Release",
                "configurations": {
                    "Debug": {
                        "defines": [
                            "DEBUG",
                        ],
                        "xcode_settings": {
                            "GCC_OPTIMIZATION_LEVEL": "0",
                            "GCC_GENERATE_DEBUGGING_SYMBOLS": "YES"
                        }
                    },
                    "Release": {
                        "defines": [
                            "NDEBUG"
                        ],
                        "xcode_settings": {
                            "GCC_OPTIMIZATION_LEVEL": "3",
                            "GCC_GENERATE_DEBUGGING_SYMBOLS": "NO",
                            "DEAD_CODE_STRIPPING": "YES",
                            "GCC_INLINES_ARE_PRIVATE_EXTERN": "YES"
                        }
                    }
                }
            }]
        ],
    },
    'targets': [
        {
            # node dmoz module
            'target_name': 'dmoz',
            'sources': [
                # core dmoz module
                'dmoz.h',
                'dmoz.cpp',
            ],
            'include_dirs': [
                'qminer/src/glib/base',
                'qminer/src/glib/mine',
                'qminer/src/nodejs/',
                'qminer/src/glib/misc/',
                'qminer/src/glib/concurrent/',
                'qminer/src/third_party/sole/',
                '<(LIN_ALG_INCLUDE)',
                '<(LIN_EIGEN_INCLUDE)'
            ],
            'dependencies': [
                'glib'
            ],
        },
        {
            # glib library
            'target_name': 'glib',
            'type': 'static_library',
            'sources': [
                'qminer/src/glib/base/base.h',
                'qminer/src/glib/base/base.cpp',
                'qminer/src/glib/mine/mine.h',
                'qminer/src/glib/mine/mine.cpp',
                'qminer/src/glib/concurrent/thread.h',
                'qminer/src/glib/concurrent/thread.cpp',
                'qminer/src/third_party/sole/sole.cpp',
                'qminer/src/third_party/libsvm/svm.h',
                'qminer/src/third_party/libsvm/svm.cpp'
            ],
            'include_dirs': [
                'qminer/src/glib/base/',
                'qminer/src/glib/mine/',
                'qminer/src/glib/misc/',
                'qminer/src/glib/concurrent/',
                'qminer/src/third_party/sole/',
                'qminer/src/third_party/libsvm/',
                '<(LIN_ALG_INCLUDE)',
                '<(LIN_EIGEN_INCLUDE)'
            ],
        },
        {
            # needed for publishing binaries with node-pre-gyp
            'target_name': 'action_after_build',
            'type': 'none',
            'dependencies': [ 'dmoz' ],
            'copies': [{
                'files': [ '<(PRODUCT_DIR)/dmoz.node' ],
                'destination': './out/'
            }]
        }
    ]
}
