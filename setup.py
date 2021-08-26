import setuptools
from distutils.core import Extension
from Cython.Build import cythonize
import numpy
from os.path import join

# get setup variables
variables = {}
with open(join('mudpy', 'global_variables.py')) as fid:
    exec(fid.read(), variables)
    
__src__ = variables['__src__']
__version__ = variables['__version__']

with open("README.md", "r", encoding="utf8") as fh:
    long_description = fh.read()
    
# module extension
ext = Extension("mudpy.mud_friendly",
                sources=[join("mudpy", "mud_friendly_wrapper.pyx"),
                         join(__src__, "mud.c"),
                         join(__src__, "mud_gen.c"),
                         join(__src__, "mud_fort.c"),
                         join(__src__, "mud_encode.c"),
                         join(__src__, "mud_all.c"),
                         join(__src__, "mud_tri_ti.c"),
                         join(__src__, "mud_misc.c"),
                         join(__src__, "mud_new.c")],
                include_dirs = [__src__, numpy.get_include()])

setuptools.setup(
    name = "mud-py",
    version = __version__,
    author = "Derek Fujimoto",
    author_email = "fujimoto@phas.ubc.ca",
    description = "MUon Data file reader",
    long_description = long_description,
    long_description_content_type = "text/markdown",
    url = "https://github.com/dfujim/mudpy",
    packages=setuptools.find_packages(),
    classifiers = [
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Cython",
        "License :: OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
        "Development Status :: 5 - Production/Stable",
    ],
    install_requires = ['cython>=0.28', 'numpy>=1.19'],
    ext_modules = cythonize([ext], 
                            include_path = [numpy.get_include()], 
                            compiler_directives={'language_level' : "3"}),
)

