project('gtkmm_calculator', 'cpp',
  version : '1.0',
  default_options : ['cpp_std=c++20']
)

gtkmm_dep = dependency('gtkmm-4.0')

executable('calculator',
  'main.cpp',
  dependencies: gtkmm_dep,
  include_directories: ['.', 'external/exprtk'],  # Ensure exprtk is found
  cpp_args: ['-Wall', '-Wextra', '-pedantic', '-O2']
)
