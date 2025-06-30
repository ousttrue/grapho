project('DirectXMath')

cpp = meson.get_compiler('cpp')
compile_args = []
if cpp.get_id() != 'msvc'
  compile_args += ['-Wno-defaulted-function-deleted']
endif

directxmath_dep = declare_dependency(
    include_directories: include_directories('Inc'),
    compile_args: compile_args,
)
