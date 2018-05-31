# SPARK & pyspk

pyspk is a Python binding of SPARK paricle engine.

 - SPARK: <https://sourceforge.net/projects/sparkengine/>


## Supported Python version

 - Python 3.6 later


\* Operation is not confirmed with 3.5 (I will take care of).  
\* 2.7 is not supported. (can't build...)

## requirements

  - [boost](https://www.boost.org/) (Mandatory)
  - OpenGL(GL, GLU) (Mandatory)
  - [pyopengl](http://pyopengl.sourceforge.net/)
  - [SDL2](https://www.libsdl.org/)
  - [pysdl2](https://pysdl2.readthedocs.io/en/rel_0_9_6/install.html)
  - [freetype2](https://www.freetype.org/)
  - [FTGL](https://sourceforge.net/projects/ftgl/)
  - [PyFTGL](https://pypi.org/project/PyFTGL/)

## Build and Install

Build process is quite difficult ( Almost because of FTGL and boost... )  
So **I strongly recommend using [anaconda](https://www.anaconda.com/) or [miniconda](https://conda.io/miniconda.html) environment**.

###  for Linux Users

  (I tested on LinuxMint 18.3 on VirtualBox with gcc 5.4)

    # create anaconda Virtual env
    $ conda create -n spk python=3.6
    $ source activate spk

    # install pyopengl
    $ sudo apt install libglu1-mesa-dev mesa-common-dev
    # pip install pyopengl

    # install SDL2
    $ sudo apt install libsdl2-dev
    $ pip install pysdl2

    # install boost
    $ conda install boost

    # install freetype2
    $ wget https://download.savannah.gnu.org/releases/freetype/freetype-2.9.tar.bz2
    $ tar xf freetype-2.9.tar.bz2
    $ cd freetype-2.9
    $ ./configure
    $ make
    $ sudo make install
    $ cd ..

    # install FTGL  (from https://sourceforge.net/projects/ftgl/)
    $ wget 'https://sourceforge.net/projects/ftgl/files/FTGL%20Source/2.1.3%7Erc5/ftgl-2.1.3-rc5.tar.bz2'
    # Depenging on your wget version, the command may fail with 'Unable to establish SSH connection.' error.
    # In that case, please download it manually via browser.
    $ tar xf ftgl-2.1.3-rc5.tar.bz2
    $ cd ftgl-2.1.3-rc5
    $ make
    $ sudo make install
    $ cd ..

    # install PyFTGL
    $ wget http://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/pyftgl/PyFTGL-0.5c.tar.bz2
    $ tar xf PyFTGL-0.5c.tar.bz2
    $ cd pyftgl
    # add the paths to freetype, boost
    $ export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/include/freetype2:$CONDA_PREFIX/include
    $ python setup.py install
    $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
    $ cd ..

    # install pyspk
    $ git clone https://github.com/chromia/pyspk.git
    $ cd pyspk
    $ python seutp.py install


###  for Window Users

 - prerequisites

    - [anaconda](https://www.anaconda.com/) or [miniconda](https://conda.io/miniconda.html)
    - ["Visual Studio 2017"](https://www.visualstudio.com/ja/downloads/) or ["Build Tools for Visual Studio 2017"(Free)](http://landinghub.visualstudio.com/visual-cpp-build-tools)  
    - [Git & Git Bash](https://gitforwindows.org/)

 Firstly, please open 'Command Prompt' by *"x64 Native Tools Command Prompt for VS 2017"* command in Start Menu:  




  * Build pyspk


    > conda create -n spkinstall python=3.6
    > conda config --add channels conda-forge
    > conda install boost=1.66.0    # 1.67 has a linking bug
    # add paths to libraries installed by conda
    > set INCLUDE=%CONDA_PREFIX%\Library\include;%INCLUDE%
    > set LIB=%CONDA_PREFIX%\Library\lib;%LIB%

    # download pyspk
    > git clone https://github.com/chromia/pyspk.git
    > cd pyspk
    > python setup.py install
    > cd ..

  * Install Additional Libraries ( this is optional, but necessary to run Demos )


    > conda install sdl2
    > conda install pysdl2
    > conda install pyopengl
    > conda install freetype


  * Build FTGL & PyFTGL ( this is optional, but necessary to run Demos )


    # download FTGL files from Git repository
    > git clone https://github.com/chromia/ftgl
    > cd ftgl
    # run msbuild
    > set UseEnv=True
    > msbuild msvc\vc2017\ftgl_dll.vcxproj /property:Configuration=release
    # create bin,include,lib directories
    > msvc\vc2017\collect.bat
    # add the paths to ftgl
    > set PATH=<absolute path to .\bin dir>;%PATH%      # should be created as permanent link
    > set INCLUDE=<absolute path to .\include dir>;%INCLUDE%
    > set LIB=<absolute path to .\lib>;%LIB%
    > cd ..

    # download PyFtgl from below URL
    # http://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/pyftgl/PyFTGL-0.5c.tar.bz2
    > tar xf PyFTGL-0.5c.tar.bz2    ( if you haven't tar command, download archiver like 7-Zip)
    > cd pyftgl
    # Editing setup.py
    # Find the line specifying OpenGL Library name
    # > module_ftgl_libs = [
    # >   'GLU',
    # >   'GL',
    # rewrite like this
    # > module_ftgl_libs = [
    # >   'glu32',
    # >   'opengl32',
    # And one more line,
    # >   'libboost_python',
    # to
    # >   'libboost_python3',
    # then, save it and close.
    > python setup.py install
    > cd ..
