import setuptools
from Cython.Build import cythonize
import numpy
from os.path import join

# get setup variables
variables = {}
with open(join('mudpy', 'global_variables.py')) as fid:
    exec(fid.read(), variables)

__src__ = variables['__src__']

# get needed compile arguments
compile_flags = ['-O3']

# module extension
ext = setuptools.Extension("mudpy.mud_friendly",
                sources=[join("mudpy", "mud_friendly_wrapper.pyx"),
                         join(__src__, "mud.c"),
                         join(__src__, "mud_gen.c"),
                         join(__src__, "mud_encode.c"),
                         join(__src__, "mud_all.c"),
                         join(__src__, "mud_tri_ti.c"),
                         join(__src__, "mud_misc.c"),
                         join(__src__, "mud_new.c")],
                include_dirs = [__src__, numpy.get_include()],
                extra_compile_args = compile_flags)

setuptools.setup(
    name = "mud-py",
    ext_modules = cythonize([ext],
                            include_path = [numpy.get_include()],
                            compiler_directives={"language_level" : "3"})
    )
