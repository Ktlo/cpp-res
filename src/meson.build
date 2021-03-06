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

lib = library(meson.project_name(), sources, dependencies: module_deps, native : get_option('native_tool'))
exe = executable(meson.project_name(), 'main.cpp', link_with : lib, install : get_option('native_tool') != true, dependencies : module_deps, native : get_option('native_tool'))
run_target('run', command : [exe] + get_option('run_args'), depends : exe)

test('run program headers', exe, args : [
  '-ns', 'resources',
  '-path', meson.source_root(),
  '-header', 'test-res.hpp',
  '-source', 'test-res.cpp',
  '-header-only'
] + sources, is_parallel : false, suite : 'unit')
test('run program sources', exe, args : [
  '-ns', 'resources',
  '-path', meson.source_root(),
  '-header', 'test-res.hpp',
  '-source', 'test-res.cpp',
  '-source-only'
] + sources, is_parallel : false, suite : 'unit')
test('run program both', exe, args : [
  '-ns', 'resources',
  '-path', meson.source_root(),
  '-header', 'test-res.hpp',
  '-source', 'test-res.cpp',
  '-tree'
] + sources, is_parallel : false, suite : 'unit')
test('print help', exe, args : [ '-help' ])
test('print version', exe, args : [ '-version' ], suite : 'unit')
