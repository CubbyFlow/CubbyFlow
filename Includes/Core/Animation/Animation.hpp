// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ANIMATION_HPP
#define CUBBYFLOW_ANIMATION_HPP

#include <Core/Animation/Frame.hpp>

#include <memory>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for animation-related class.
//!
//! This class represents the animation logic in very abstract level.
//! Generally animation is a function of time and/or its previous state.
//! This base class provides a virtual function Update() which can be
//! overriden by its sub-classes to implement their own state update logic.
//!
class Animation
{
 public:
    //! Default constructor.
    Animation() = default;

    //! Default copy constructor.
    Animation(const Animation&) = default;

    //! Default move constructor.
    Animation(Animation&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Animation() = default;

    //! Default copy assignment operator.
    Animation& operator=(const Animation&) = default;

    //! Default move assignment operator.
    Animation& operator=(Animation&&) noexcept = default;

    //!
    //! \brief Updates animation state for given \p frame.
    //!
    //! This function updates animation state by calling Animation::OnUpdate
    //! function.
    //!
    void Update(const Frame& frame);

 protected:
    //!
    //! \brief The implementation of this function should update the animation
    //! state for given Frame instance \p frame.
    //!
    //! This function is called from Animation::Update when state of this class
    //! instance needs to be updated. Thus, the inherited class should override
    //! this function and implement its logic for updating the animation state.
    //!
    virtual void OnUpdate(const Frame& frame) = 0;
};

//! Shared pointer for the Animation type.
using AnimationPtr = std::shared_ptr<Animation>;
}  // namespace CubbyFlow

#endif