// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_SPH_SIM_EXAMPLE_HPP
#define CUBBYFLOW_CUDA_SPH_SIM_EXAMPLE_HPP

#include <string>

namespace CubbyFlow
{
class SPHSimExample
{
 public:
    virtual void RunExample(const std::string& rootDir, float targetSpacing,
                            int numberOfFrames, const std::string& format,
                            double fps);
};

}  // namespace CubbyFlow

#endif