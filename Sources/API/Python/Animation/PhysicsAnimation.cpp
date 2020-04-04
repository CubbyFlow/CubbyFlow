// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Animation/PhysicsAnimation.hpp>
#include <Core/Animation/Animation.hpp>
#include <Core/Animation/PhysicsAnimation.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

class PyPhysicsAnimation : public PhysicsAnimation
{
 public:
    using PhysicsAnimation::PhysicsAnimation;

    void OnAdvanceTimeStep(double timeIntervalInSeconds) override
    {
        PYBIND11_OVERLOAD_PURE(void, PhysicsAnimation, OnAdvanceTimeStep,
                               timeIntervalInSeconds);
    }

    unsigned int GetNumberOfSubTimeSteps(
        double timeIntervalInSeconds) const override
    {
        PYBIND11_OVERLOAD(unsigned int, PhysicsAnimation,
                          GetNumberOfSubTimeSteps, timeIntervalInSeconds);
    }

    void OnInitialize() override
    {
        PYBIND11_OVERLOAD(void, PhysicsAnimation, OnInitialize, );
    }
};

void AddPhysicsAnimation(pybind11::module& m)
{
    pybind11::class_<PhysicsAnimation, PyPhysicsAnimation, PhysicsAnimationPtr,
                     Animation>(static_cast<pybind11::handle>(m),
                                "PhysicsAnimation")
        .def(pybind11::init<>())
        .def_property("isUsingFixedSubTimeSteps",
                      &PhysicsAnimation::GetIsUsingFixedSubTimeSteps,
                      &PhysicsAnimation::SetIsUsingFixedSubTimeSteps)
        .def_property("numberOfFixedSubTimeSteps",
                      &PhysicsAnimation::GetNumberOfFixedSubTimeSteps,
                      &PhysicsAnimation::SetNumberOfFixedSubTimeSteps)
        .def("AdvanceSingleFrame", &PhysicsAnimation::AdvanceSingleFrame)
        .def_property("currentFrame", &PhysicsAnimation::GetCurrentFrame,
                      &PhysicsAnimation::SetCurrentFrame)
        .def_property_readonly("currentTimeInSeconds",
                               &PhysicsAnimation::GetCurrentTimeInSeconds);
}