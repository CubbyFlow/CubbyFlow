// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.
#include <Core/Utils/Logging.h>
#include <Core/Utils/Timer.h>
#include <Core/Animation/Animation.hpp>

namespace CubbyFlow
{
Animation::Animation()
{
    // Do nothing
}

Animation::~Animation()
{
    // Do nothing
}

void Animation::Update(const Frame& frame)
{
    Timer timer;

    CUBBYFLOW_INFO << "Begin updating frame: " << frame.index
                   << " timeIntervalInSeconds: " << frame.timeIntervalInSeconds
                   << " (1/" << 1.0 / frame.timeIntervalInSeconds
                   << ") seconds";

    OnUpdate(frame);

    CUBBYFLOW_INFO << "End updating frame (took " << timer.DurationInSeconds()
                   << " seconds)";
}
}  // namespace CubbyFlow