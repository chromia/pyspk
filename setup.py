from setuptools import setup, Extension, find_packages
from pathlib import Path
import sys
import os


def grepfiles(rootdir, cond):
    p = Path(rootdir)
    return [str(v) for v in list(p.glob(cond))]


# Listing cpp source files
spark_core = grepfiles('./spark/src/Core', '**/*.cpp')
spark_extensions = grepfiles('./spark/src/Extensions', '**/*.cpp')
spark_gl = grepfiles('./spark/src/RenderingAPIs/OpenGL', '**/*.cpp')
pyspk_main = grepfiles('./pyspk/src/pyspk_main/', '**/*.cpp')
pyspk_gl = grepfiles('./pyspk/src/pyspk_GL/', '**/*.cpp')
main_sources = spark_core + spark_extensions + pyspk_main
gl_sources = spark_core + spark_extensions + spark_gl + pyspk_gl  # quite redundunt

include_dirs = ['./spark/include']
library_dirs = []

if os.name == 'nt':
    # Windows( expect Visual C++ )
    extra_compile_args = []  # c++11 is default
    main_libraries = []
    GL_libraries = ['opengl32', 'glu32']  # boost is linked automatically
else:
    # POSIX OS( expect GCC )
    # solving boost library name
    #   boost 1.66 or ealier : libboost_python3
    #   boost 1.67 or later  : libboost_python35 (or 36 or 37 ...)
    # is there an easy method to get boost version?
    boost_lib = os.getenv('BOOST_LIB_NAME', None)
    if boost_lib is None:
        # get default boost libname(e.g. boost_python35/boost_python36/...)
        boost_lib = 'boost_python' + sys.version_info.major + sys.version_info.minor
    extra_compile_args = ['-std=c++11']
    main_libraries = [boost_lib]
    GL_libraries = ['GL', 'GLU', boost_lib]


ext_main = Extension('pyspk._pyspk', main_sources,
                     include_dirs=include_dirs,
                     library_dirs=library_dirs,
                     libraries=main_libraries,
                     language='c++',
                     extra_compile_args=extra_compile_args)
ext_GL = Extension('pyspk.GL._GL', gl_sources,
                   include_dirs=include_dirs,
                   library_dirs=library_dirs,
                   libraries=GL_libraries,
                   language='c++',
                   extra_compile_args=extra_compile_args)

setup(name='pyspk',
      version='0.1.0',
      url='https://github.com/chromia/pyspk',
      author='chromia',
      author_email='chromia@outlook.jp',
      license='zlib',
      description='pyspk: a binding of SPARK particle engine',
      long_description='file: README.md',
      packages=find_packages(),
      ext_modules=[ext_main, ext_GL])
