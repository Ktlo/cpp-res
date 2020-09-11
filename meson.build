project('cpp-res', 'cpp',
  version : run_command('git', 'describe', '--abbrev=0', '--tags').stdout().strip(),
  default_options : ['warning_level=3',
                     'cpp_std=c++17',
                     'build.cpp_std=c++17'])

group = 'com.handtruth'
maintainer = 'ktlo <ktlo@handtruth.com>'

modules = [
]

######################################

module_deps = []

foreach module : modules
  module_deps += dependency(module, fallback : [module, 'dep'])
endforeach

fslib = meson.get_compiler('cpp', native : get_option('native_tool')).find_library('stdc++fs', required : false)
if fslib.found()
  module_deps += fslib
endif

subdir('src')
subdir('test')

cppcheck = custom_target(meson.project_name() + '_cppcheck_internal',
  output : meson.project_name() + '_cppcheck.log',
  input : sources + test_files,
  command : [
    'cppcheck',
    '--enable=all',
    '-I', meson.current_source_dir() / 'include',
    '-I', meson.current_source_dir() / 'src',
    '@INPUT@',
#   '--project=compile_commands.json',
    '--output-file=@OUTPUT@'
])

run_target('cppcheck', command : ['cat', cppcheck.full_path()], depends : cppcheck)
