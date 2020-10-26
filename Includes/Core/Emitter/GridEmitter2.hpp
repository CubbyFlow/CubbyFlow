// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_EMITTER2_HPP
#define CUBBYFLOW_GRID_EMITTER2_HPP

#include <functional>
#include <memory>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D grid-based emitters.
//!
class GridEmitter2
{
 public:
    //!
    //! \brief Callback function type for update calls.
    //!
    //! This type of callback function will take the emitter pointer, current
    //! time, and time interval in seconds.
    //!
    using OnBeginUpdateCallback =
        std::function<void(GridEmitter2*, double, double)>;

    //! Default constructor.
    GridEmitter2() = default;

    //! Default copy constructor.
    GridEmitter2(const GridEmitter2&) = default;

    //! Default move constructor.
    GridEmitter2(GridEmitter2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~GridEmitter2() = default;

    //! Default copy assignment operator.
    GridEmitter2& operator=(const GridEmitter2&) = default;

    //! Default copy assignment operator.
    GridEmitter2& operator=(GridEmitter2&&) noexcept = default;

    //! Updates the emitter state from \p currentTimeInSeconds to the following
    //! time-step.
    void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

    //! Returns true if the emitter is enabled.
    [[nodiscard]] bool GetIsEnabled() const;

    //! Sets true/false to enable/disable the emitter.
    void SetIsEnabled(bool enabled);

    //!
    //! \brief      Sets the callback function to be called when
    //!             GridEmitter2::Update function is invoked.
    //!
    //! The callback function takes current simulation time in seconds unit. Use
    //! this callback to track any motion or state changes related to this
    //! emitter.
    //!
    //! \param[in]  callback The callback function.
    //!
    void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

 protected:
    virtual void OnUpdate(double currentTimeInSeconds,
                          double timeIntervalInSeconds) = 0;

    void CallOnBeginUpdateCallback(double currentTimeInSeconds,
                                   double timeIntervalInSeconds);

 private:
    OnBeginUpdateCallback m_onBeginUpdateCallback;
    bool m_isEnabled = true;
};

//! Shared pointer type for the GridEmitter2.
using GridEmitter2Ptr = std::shared_ptr<GridEmitter2>;
}  // namespace CubbyFlow

#endif