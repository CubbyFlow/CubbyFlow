// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Animation/Animation.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Timer.hpp>

namespace CubbyFlow
{
void Animation::Update(const Frame& frame)
{
    const Timer timer;

    CUBBYFLOW_INFO << "Begin updating frame: " << frame.index
                   << " timeIntervalInSeconds: " << frame.timeIntervalInSeconds
                   << " (1/" << 1.0 / frame.timeIntervalInSeconds
                   << ") seconds";

    OnUpdate(frame);

    CUBBYFLOW_INFO << "End updating frame (took " << timer.DurationInSeconds()
                   << " seconds)";
}
}  // namespace CubbyFlow