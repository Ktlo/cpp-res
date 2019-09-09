sources = files([
  'scanner.cpp',
  'arguments.cpp',
  'cppres.cpp'
])

src = include_directories('.')

sources += configure_file(output : 'config.cpp', input : 'config.cpp.in',
  configuration : {
  'group' : group,
  'project' : meson.project_name(),
  'version' : meson.project_version(),
  'build' : run_command('git', 'describe').stdout().strip(),
  'maintainer' : maintainer,
})

static_lib = static_library(meson.project_name(), sources, dependencies: module_deps)
exe = executable(meson.project_name(), 'main.cpp', link_with : static_lib, install : true, dependencies : module_deps)
run_target('run', command : [exe] + get_option('run_args'), depends : exe)

test('run program headers', exe, args : [
  '-ns', 'resources',
  '-path', meson.source_root(),
  '-header', 'test-res.hpp',
  '-source', 'test-res.cpp',
  '-header-only'
] + sources, is_parallel : false)
test('run program sources', exe, args : [
  '-ns', 'resources',
  '-path', meson.source_root(),
  '-header', 'test-res.hpp',
  '-source', 'test-res.cpp',
  '-source-only'
] + sources, is_parallel : false)
test('run program both', exe, args : [
  '-ns', 'resources',
  '-path', meson.source_root(),
  '-header', 'test-res.hpp',
  '-source', 'test-res.cpp',
  '-tree'
] + sources, is_parallel : false)
test('print help', exe, args : [ '-help' ])
test('print version', exe, args : [ '-version' ])
