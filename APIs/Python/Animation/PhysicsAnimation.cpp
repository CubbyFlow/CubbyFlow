/*************************************************************************
> File Name: PhysicsAnimation.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Physics animation functions for CubbyFlow Python API.
> Created Time: 2018/01/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Animation/PhysicsAnimation.h>
#include <Animation/Animation.h>
#include <Animation/PhysicsAnimation.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

class PyPhysicsAnimation : public PhysicsAnimation
{
public:
    using PhysicsAnimation::PhysicsAnimation;

    void OnAdvanceTimeStep(double timeIntervalInSeconds) override
    {
        PYBIND11_OVERLOAD_PURE(void, PhysicsAnimation, OnAdvanceTimeStep, timeIntervalInSeconds);
    }

    unsigned int GetNumberOfSubTimeSteps(double timeIntervalInSeconds) const override
    {
        PYBIND11_OVERLOAD(unsigned int, PhysicsAnimation, GetNumberOfSubTimeSteps, timeIntervalInSeconds);
    }

    void OnInitialize() override
    {
        PYBIND11_OVERLOAD(void, PhysicsAnimation, OnInitialize, );
    }
};

void AddPhysicsAnimation(pybind11::module& m)
{
    pybind11::class_<PhysicsAnimation, PyPhysicsAnimation, PhysicsAnimationPtr, Animation>(static_cast<pybind11::handle>(m), "PhysicsAnimation")
    .def(pybind11::init<>())
    .def_property("isUsingFixedSubTimeSteps", &PhysicsAnimation::GetIsUsingFixedSubTimeSteps, &PhysicsAnimation::SetIsUsingFixedSubTimeSteps)
    .def_property("numberOfFixedSubTimeSteps", &PhysicsAnimation::GetNumberOfFixedSubTimeSteps, &PhysicsAnimation::SetNumberOfFixedSubTimeSteps)
    .def("AdvanceSingleFrame", &PhysicsAnimation::AdvanceSingleFrame)
    .def_property("currentFrame", &PhysicsAnimation::GetCurrentFrame, &PhysicsAnimation::SetCurrentFrame)
    .def_property_readonly("currentTimeInSeconds", &PhysicsAnimation::GetCurrentTimeInSeconds);
}