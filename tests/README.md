# Building Tests for Windows

Requires

* Ninja
* Visual Studio 2019 Community Edition

First, start a `cmd` terminal, and set the visual studio environment
variables, by running:

    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

Then, execute CMake:

    cd tinker\tests
    mkdir build
    cd build
    cmake -G Ninja ..
    ninja && ctest . --verbose