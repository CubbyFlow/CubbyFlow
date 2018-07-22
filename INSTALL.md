This document explains how to build, test, and install the SDK.

## Building the Code

To get started, clone the code from the repository and also download dependent libraries by running

```
git clone https://github.com/utilForever/CubbyFlow-v0.git
cd CubbyFlow-v0.git
git submodule init
git submodule update
```

To build the code, a compiler that supports C++17 is required. Platform-specific build instructions are described below.

### Building from macOS

CubbyFlow-v0 supports OS X 10.12.6 Sierra or higher. Also, Xcode 9 or higher and the command line tools are required for building CubbyFlow-v0. Once ready, install [Homebrew](http://brew.sh) and run the following command line to setup [CMake](https://cmake.org/):

```
brew install cmake python
```

> Note that we want `brew` version of Python which is recommended. You can still use macOS's default Python.

Once CMake is installed, build the code by running

```
mkdir build
cd build
cmake ..
make
```

> Of course, use `make -j <num_threads>` flag to boost up the build performance by using multithreads.

This will build entire codebase. To run the unit test, execute

```
bin/UnitTests
```

It should show all the tests are passing.

### Building from Ubuntu

CubbyFlow-v0 supports Ubuntu 17.04 or higher. Using `apt-get`, install required tools and libraries by running,

```
sudo apt-get install build-essential python-dev python-pip cmake
```

This will install GNU compilers, Python, and CMake. Once installed, build the code by running

```
mkdir build
cd build
cmake ..
make
```

> Again, use `make -j <num_threads>` flag to boost up the build performance by using multithreads.

This will build entire codebase. To run the unit test, execute

```
bin/UnitTests
```

It should show all the tests are passing.

### Building from Windows

To build the code on Windows, CMake, Python, and Visual Studio 2017 (or higher) is required. Windows' version of CMake is available from [this website](https://cmake.org/), Python installer can be downloaded from [here](https://python.org/). For Python, version 2.7.9 or later is recommended. To install Visual Studio, the community edition of the tool can be downloaded from [Visual Studio Community 2017](https://www.Visualstudio.com/en-us/products/Visual-studio-community-vs.aspx).

Once everything is installed, run the following commands:

```
md build
cd build
cmake .. -G"Visual Studio 15 2017 Win64"
```

This will generate 64-bit version of VS 2017 solution and projects. Once executed, you can find `CubbyFlow-v0.sln` solution file in the `build` directory. Open the solution file and hit `Ctrl + Shift + B` to build the entire solution. Set `UnitTests` as a start-up project and hit `Ctrl + F5` to run the test.

Alternatively, you can use MSBuild to build the solution from the command prompt. In such case, simply run:

```
MSBuild CubbyFlow-v0.sln /p:Configuration=Release
```

This will build the whole solution in release mode. Once built, run the following command to execute unit tests:

```
bin\Release\UnitTests.exe
```

### Building from Windows Subsystem for Linux (WSL)

To build the code on Windows Subsystem for Linux, CMake, Python, and GCC 7.1 is required. Linux' version of CMake is available from [this website](https://cmake.org/), Python installer can be downloaded from [here](https://python.org/). For Python, version 2.7.9 or later is recommended. GCC 7.1 can be downloaded from [here](http://gcc.parentingamerica.com/releases/gcc-7.1.0/gcc-7.1.0.tar.bz2).

Currently, GCC 5.4 is installed on WSL. GCC 7 or later must be installed to use C++17 features. To install GCC 7.1, use these instructions manually.

1. Start WSL (use the Bash on Ubuntu on Windows console or write bash in a Command Prompt window) and write

    ```
    sudo apt update
    sudo apt upgrade
    ```

2. Install the default GCC toolchain with:

    ```
    sudo apt install build-essential
    ```

3. Download the GCC 7.1 source and prerequisites from http://gcc.gnu.org/mirrors.html:

    ```
    cd ~
    wget http://gcc.parentingamerica.com/releases/gcc-7.1.0/gcc-7.1.0.tar.bz2
    tar xf gcc-7.1.0.tar.bz2
    cd gcc-7.1.0
    contrib/download_prerequisites
    ```

4. At this point, we can configure the build. In order to keep the system clean, we will use /usr/local/gcc-7.1 for the installation folder and append the suffix -7.1 to the GCC compilers. You typically don’t want to mess the system’s default GCC because other packages may depend on this.

    ```
    cd ~
    mkdir build && cd build
    ../gcc-7.1.0/configure -v --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --prefix=/usr/local/gcc-7.1 --enable-checking=release --enable-languages=c,c++,fortran --disable-multilib --program-suffix=-7.1
    ```

5. We are ready to build GCC, you typically want to pass twice the number of your computer cores to the make command in order to speed up the build. I have a quad-core system, so I will use 8 parallel jobs to build GCC:

    ```
    make -j 8
    ```

    NOTE: Depending on the speed of your computer the build phase could take from about 30 minutes to a few hours.

6. Once the above phase is finished, you can install the built GCC with:

    ```
    sudo make install
    ```

7. If you want to permanently add the compilers to your system’s path, use the next commands:

    ```
    cd ~
    echo 'export PATH=/usr/local/gcc-7.1/bin:$PATH' >> .bashrc
    echo 'export LD_LIBRARY_PATH=/usr/local/gcc-7.1/lib64:$LD_LIBRARY_PATH' >> .bashrc
    source .bashrc
    ```

    NOTE: The above will append the path to GCC 7.1 at the end of your .bashrc file.

Also, CMake is not installed on WSL. CMake 3.8.2 or later must be installed to use C++17 features. To install CMake 3.10.1, use these instructions manually.

1. Download the CMake 3.10.1 source from https://cmake.org/download/:

    ```
    cd ~
    wget https://cmake.org/files/v3.10/cmake-3.10.1.tar.gz
    tar xf cmake-3.10.1.tar.gz
    cd cmake-3.10.1
    ```

2. Prepare to build using the bootstrap script with:

    ```
    ./bootstrap --prefix=/usr
    ```

    NOTE: ```--prefix=/usr``` is used to specify the CMake path on the WSL.

3. We are ready to build CMake, you typically want to pass twice the number of your computer cores to the make command in order to speed up the build. I have a quad-core system, so I will use 8 parallel jobs to build CMake:

    ```
    make -j 8
    ```

4. Once the above phase is finished, you can install the built CMake with:

    ```
    sudo make install
    ```

Once everything is installed, add the following line to CMakeLists.txt using ```vi```:

```
vi CMakeLists.txt
```

NOTE: Please move to CubbyFlow-v0 folder.

```
# CMake version
cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

# ADD THE FOLLOWING LINE
set(CMAKE_C_COMPILER "gcc-7.1")
set(CMAKE_CXX_COMPILER "g++-7.1")
# ADD THE FOLLOWING LINE

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Builds/CMake")
```

Finally, ready to build CubbyFlow-v0. Run the following commands:

```
mkdir build
cd build
cmake ..
make
```

> Again, use `make -j <num_threads>` flag to boost up the build performance by using multithreads.

This will build entire codebase. To run the unit test, execute

```
bin/UnitTests
```

It should show all the tests are passing.

### Running Tests

There are two different tests in the codebase including the unit test and manual test. For the detailed instruction on how to run those tests, please checkout the documentation page from [the project website](https://utilforever.github.io/CubbyFlow-v0/Documentation/).

### Code Coverage

CubbyFlow-v0 uses ```lcov``` for the code coverage. For macOS and Ubuntu platforms, the code coverage report can be generated by running

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j 8
lcov -c -i -d Tests/UnitTests -o base.info
bin/UnitTests
lcov -c -d Tests/UnitTests -o test.info
lcov -a base.info -a test.info -o coverage.info
lcov -r coverage.info '/usr/*' -o coverage.info
lcov -r coverage.info '*/Libraries/*' -o coverage.info
lcov -r coverage.info '*/Tests/*' -o coverage.info
lcov -l coverage.info
genhtml coverage.info -o out
```

This will exports the code coverage report ```index.html``` under `out` folder.

### Installing C++ SDK

For macOS and Ubuntu platforms, the library can be installed by running

```
cmake .. -DCMAKE_INSTALL_PREFIX=_INSTALL_PATH_
make
make install
```

This will install the header files and the static library `libCubbyFlow-v0.a` under `_INSTALL_PATH_`.

For Windows, run:

```
cmake .. -G"Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=_INSTALL_PATH_
```

Then, build `INSTALL` project under `CubbyFlow-v0.sln`. This will install the header files and the static library `CubbyFlow-v0.lib` under `_INSTALL_PATH_`.

### Installing Python SDK

To install the Python SDK, `pyCubbyFlow-v0`, run the following command from the project root directory (where `setup.py` lives):

```
pip install -U .
```

> You can also use `virtualenv` to isolate the SDK installation. Check out [the virtualenv documentation](https://virtualenv.pypa.io/en/stable/) for more details.

To run the test scripts, install other Python dependencies as follows:

```
pip install -r Requirements.txt
```

Once installed, try running the unit test to see if the module is installed correctly:

```
python Tests/PythonTests/main.py
```

The tests should pass.

### Coding Style

CubbyFlow-v0 uses clang-format. Checkout [`.clang-format`](https://github.com/utilforever/CubbyFlow-v0/blob/master/.clang-format) file for the style guideline.