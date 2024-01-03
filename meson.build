project('mud-py', 'cython', 'c',
    license: 'LGPLv3',
    default_options: [
        'buildtype=release',
    ],
)

# load python module
py_mod = import('python')
py = py_mod.find_installation(pure: false, required: true, modules: 'numpy')
py_dep = py.dependency()

# get numpy include directory
incdir_numpy = meson.get_external_property('numpy-include-dir', 'not-given')

if incdir_numpy == 'not-given'
    incdir_numpy = run_command(py,
        [
        '-c',
        '''import numpy
incdir = numpy.get_include()
print(incdir)
        '''
        ],
        check: true
    ).stdout().strip()
endif

# run subdirectories
subdir('mud_src')
subdir('mudpy')