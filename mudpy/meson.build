# install cython packages
py.extension_module(
    'mud_friendly_wrapper',
    'mud_friendly_wrapper.pyx',
    install: true,
    dependencies: py_dep,
    include_directories: ['../mud_src', incdir_numpy],
    subdir: 'mudpy',
    link_with: [mud_lib],
    override_options: ['cython_version=3'],
)

# install python packages
python_sources = [
    'containers.py',
    'global_variables.py',
    '__init__.py',
    'mcomment.py',
    'mcontainer.py',
    'mdata.py',
    'mdict.py',
    'mhist.py',
    'mlist.py',
    'mscaler.py',
    'mvar.py',
]

py.install_sources(
    python_sources,
    subdir: 'mudpy'
)