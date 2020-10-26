// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Emitter/GridEmitter2.hpp>

namespace CubbyFlow
{
void GridEmitter2::Update(double currentTimeInSeconds,
                          double timeIntervalInSeconds)
{
    if (m_onBeginUpdateCallback)
    {
        m_onBeginUpdateCallback(this, currentTimeInSeconds,
                                timeIntervalInSeconds);
    }

    OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

bool GridEmitter2::GetIsEnabled() const
{
    return m_isEnabled;
}

void GridEmitter2::SetIsEnabled(bool enabled)
{
    m_isEnabled = enabled;
}

void GridEmitter2::SetOnBeginUpdateCallback(
    const OnBeginUpdateCallback& callback)
{
    m_onBeginUpdateCallback = callback;
}

void GridEmitter2::CallOnBeginUpdateCallback(double currentTimeInSeconds,
                                             double timeIntervalInSeconds)
{
    m_onBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
}
}  // namespace CubbyFlow