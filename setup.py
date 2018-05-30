from distutils.core import setup, Extension
from pathlib import Path
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


# Edit this line to adjust to your environment(Linux)
# To find your boost library name, use this command:
# sudo find / -name libboost_python*.so
# if result is 'libboost_python36.so', then library name is 'libboost_python36'
boost_lib = ['boost_python']


if os.name == 'nt':
    # Windows( expect Visual C++ )
    extra_compile_args = []  # c++11 is default
    main_libraries = []
    GL_libraries = ['opengl32','glu32']  # boost is linked automatically
else:
    # POSIX OS( expect GCC )
    extra_compile_args = ['-std=c++11']
    main_libraries = boost_lib
    GL_libraries = ['GL', 'GLU'] + boost_lib


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
      py_modules=['pyspk/__init__', 'pyspk/GL/__init__'],
      ext_modules=[ext_main, ext_GL])
