project('Snek', 'cpp', default_options: ['cpp_std=c++20'])

sfml = dependency('sfml-graphics')

compiler_options = ['-Wall', '-Wextra']

# Define the source files and their directories
sources = files(
  'src/main.cpp',
  'src/game.cpp'
)

# Define the compiler arguments
debug_args = [
  '-Og',
  '-g',
  '-gsplit-dwarf'
]

release_args = [
  '-Os',
  '-flto',
  '-fno-gcse',
]

if get_option('buildtype') == 'debug'
    compiler_options += debug_args
else
    compiler_options += release_args
endif

# Define the output executable
executable('snek', sources : sources,
    dependencies : [sfml], cpp_args : compiler_options
)