# CubbyFlow-v0

<img src="https://github.com/utilForever/CubbyFlow-v0/blob/master/Logo.png" align="center" />

[![License](https://img.shields.io/badge/Licence-MIT-blue.svg)](https://github.com/utilForever/CubbyFlow-v0/blob/master/LICENSE) [![Build Status](https://travis-ci.org/utilForever/CubbyFlow-v0.svg?branch=master)](https://travis-ci.org/utilForever/CubbyFlow-v0/branches) [![Build status](https://ci.appveyor.com/api/projects/status/github/utilForever/CubbyFlow-v0?branch=master&svg=true)](https://ci.appveyor.com/project/utilForever/CubbyFlow-v0/branch/master)

<b>IMPORTANT: Most of the code in this repository is copied from [Jet Framework](https://github.com/doyubkim/fluid-engine-dev) and ["Fluid Engine Development" book](http://fluidenginedevelopment.org/) that was created by [Doyub Kim](https://twitter.com/doyub).</b>

CubbyFlow-v0 is full source codes of voxel-based fluid simulation engine for computer games called [CubbyFlow](http://github.com/utilForever/CubbyFlow). It is based on [Jet framework](https://github.com/doyubkim/fluid-engine-dev) that was created by [Doyub Kim](https://twitter.com/doyub).
The code is built on C++17 and can be compiled with commonly available compilers such as g++, clang++, or Microsoft Visual Studio. CubbyFlow-v0 currently supports macOS (10.12.6 or later), Ubuntu (17.04 or later), Windows (Visual Studio 2017 or later), and Windows Subsystem for Linux (WSL). Other untested platforms that support C++17 also should be able to build CubbyFlow-v0.

## Key Features

- Basic math and geometry operations and data structures
- Spatial query accelerators
- SPH and PCISPH fluid simulators
- Stable fluids-based smoke simulator
- Level set-based liquid simulator
- PIC, FLIP, and APIC fluid simulators
- Upwind, ENO, and FMM level set solvers
- Jacobi, Gauss-Seidel, SOR, MG, CG, ICCG, and MGPCG linear system solvers
- Spherical, SPH, Zhu & Bridson, and Anisotropic kernel for points-to-surface converter
- Converters between signed distance function and triangular mesh
- C++ and Python API
- Intel TBB, OpenMP, and C++11 multi-threading backends

## To-do Features

- MPS and MPM fluid simulators
- IVOCK fluid simulators
- Multiphase fluid simulators
    - Particle-based multiphase flow
    - Grid-based two-phase flow
    - Grid-based multiphase flow
    - Grid-based bubbly flow
- Voxel surface reconstruction
- Compatible for DirectX, OpenGL, Metal, and Vulkan
- C#, Go, and Rust API

Every simulator has both 2-D and 3-D implementations.

## Quick Start

You will need CMake to build the code. If you're using Windows, you need Visual Studio 2017 in addition to CMake.

First, clone the code:

```
git clone https://github.com/utilForever/CubbyFlow-v0.git --recursive
cd CubbyFlow-v0
```

### C++ API

For macOS or Linux or Windows Subsystem for Linux (WSL):

```
mkdir build
cd build
cmake ..
make
```

For Windows:

```
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64"
MSBuild CubbyFlow-v0.sln /p:Configuration=Release
```

Now run some examples, such as:

```
bin/HybridLiquidSim
```

### Python API

Build and install the package by running

```
pip install -U .
```

### More Instructions of Building the Code

To learn how to build, test, and install the SDK, please check out [INSTALL.md](https://github.com/utilForever/CubbyFlow-v0/blob/master/INSTALL.md).

## Documentation

All the documentations for the framework can be found from [the project website](https://utilforever.github.io/CubbyFlow-v0/) incluing the API reference.

## Examples

Here are some of the example simulations generated using CubbyFlow-v0 framework. Corresponding example codes can be found under [Examples](https://github.com/utilForever/CubbyFlow-v0/tree/master/Examples). All images are rendered using [Mitsuba renderer](https://www.mitsuba-renderer.org/) and the Mitsuba scene files can be found from [the demo directory](https://github.com/utilForever/CubbyFlow-v0/tree/master/Demos). Find out more demos from [the project website](https://utilforever.github.io/CubbyFlow-v0/Examples).

#### PCISPH Simulation Example

![PCISPH_dam_breaking](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/PCISPH_dam_breaking.png "PCISPH Example")

#### Level Set Simulation Example

![Level-set_dam_breaking](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/LevelSet_dam_breaking.png "Level Set Example")

#### FLIP Simulation Example

![FLIP_dam_breaking](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/FLIP_dam_breaking.png "FLIP Example")

#### PIC Simulation Example

![PIC_dam_breaking](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/PIC_dam_breaking.png "PIC Example")

#### APIC Simulation Example

![APIC_dam_breaking](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/APIC_dam_breaking.png "APIC Example")

#### Level Set Example with Different Viscosity (high / low)

![level_set_bunny_drop_high_viscosity](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/level_set_bunny_drop_high_viscosity.png "Level Set Bunny Drop - High Viscosity")
![level_set_bunny_drop_low_viscosity](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/level_set_bunny_drop_low_viscosity.png "Level Set Bunny Drop - Low Viscosity")

#### Smoke Simulation with Different Advection Methods (Linear / Cubic-Spline)

![rising_smoke_linear](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/rising_smoke_linear.png "Rising Smoke - Linear")
![rising_smoke_cubic](https://github.com/utilForever/CubbyFlow-v0/blob/master/Medias/Screenshots/rising_smoke_cubic.png "Rising Smoke - Cubic")

## Presentations

### Korean

[[NDC 2018] 유체역학 엔진 개발기 - 게임에서 물리 기반의 유체를 구현하기 위한 고군분투](https://www.slideshare.net/utilforever/ndc-2018-95260566)

#### Related articles

- http://www.gamevu.co.kr/news/articleView.html?idxno=8464

- http://www.inven.co.kr/webzine/news/?news=198413&iskin=l2mobile

### English

Coming soon...

## How To Contribute

Contributions are always welcome, either reporting issues/bugs or forking the repository and then issuing pull requests when you have completed some additional coding that you feel will be beneficial to the main project. If you are interested in contributing in a more dedicated capacity, then please contact me.

## Contact

You can contact me via e-mail (utilForever at gmail.com). I am always happy to answer questions or help with any issues you might have, and please be sure to share any additional work or your creations with me, I love seeing what other people are making.

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2018 [Chris Ohk](http://www.github.com/utilForever), [Dongmin Kim](https://github.com/dmk98), and [YoungJoong Kim](https://github.com/revsic).

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.