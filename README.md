# SPARK & pyspk

![alt spk-demo-images](http://chromia.cocotte.jp/monooki/files/ss/pyspk_ss01.jpg)


pyspk is a Python binding of SPARK particle engine.

 - SPARK: <https://sourceforge.net/projects/sparkengine/>


## Supported Python version

 - Python 3,5 or 3.6

\* 2.7 is not supported. (can't build...)

## Requirements

  - [boost](https://www.boost.org/)
  - OpenGL(GL, GLU)
  - [pyopengl](http://pyopengl.sourceforge.net/)
  - [SDL2](https://www.libsdl.org/)
  - [pysdl2](https://pysdl2.readthedocs.io/en/rel_0_9_6/install.html)

## Install using prebuilt binaries

Build process is quite difficult.  
So I have prepared wheel binaries for Python 3.5&3.6 on x64 platform. => [donwload site](http://chromia.cocotte.jp/monooki/pyspk/)  
For that reason, **I strongly recommend using [anaconda](https://www.anaconda.com/) or [miniconda](https://conda.io/miniconda.html) environment**.

###  for Linux Users

Install boost & pyspk

    $ sudo apt install libglu1-mesa-dev mesa-common-dev
    $ conda config --add channels conda-forge

    $ conda create -n spk python=3.6
    $ source activate spk
    $ conda install boost=1.66
    $ wget http://chromia.cocotte.jp/monooki/files/pyspk/pyspk-0.1.0-cp36-cp36m-linux_x86_64.whl
    $ pip install pyspk-0.1.0-cp36-cp36m-linux_x86_64.whl

Install Additional Libraries ( this is optional, but necessary to run Demos )

    $ conda install sdl2 pysdl2 pyopengl



###  for Windows Users

Install boost & pyspk

    > conda config --add channels conda-forge

    > conda create -n spk python=3.6
    > conda activate spk
    > conda install boost=1.66
    # download pyspk binary via browser (or wget/curl if you have)
    # http://chromia.cocotte.jp/monooki/files/pyspk/pyspk-0.1.0-cp36-cp36m-win_amd64.whl
    > pip install pyspk-0.1.0-cp36-cp36m-win_amd64.whl

Install Additional Libraries ( this is optional, but necessary to run Demos )

    > conda install sdl2 pysdl2 pyopengl


## How to build manually

see [./BUILD](https://github.com/chromia/pyspk/blob/master/BUILD.txt) file


## How to run Demos

On Linux, additional path settings may be required.

    # Linux: add the path to dependent libraries
    $ export LD_LIBRARY_PATH=$CONDA_PREFIX/lib:$LD_LIBRARY_PATH

Please move to 'demos' directory, and run it.

    > cd pyspk/demos
    > python basicdemo.py

  see also [demos/README.md](https://github.com/chromia/pyspk/blob/master/demos/README.md)


## License

 * zlib/libpng license  (same as SPARK)  
