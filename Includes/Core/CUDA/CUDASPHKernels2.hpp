// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_SPH_KERNELS2_HPP
#define CUBBYFLOW_CUDA_SPH_KERNELS2_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
//!
//! \brief Standard 2-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct CUDASPHStdKernel2
{
    //! Constructs a kernel object with zero radius.
    CUBBYFLOW_CUDA_HOST_DEVICE CUDASPHStdKernel2();

    //! Constructs a kernel object with given radius.
    CUBBYFLOW_CUDA_HOST_DEVICE explicit CUDASPHStdKernel2(float kernelRadius);

    //! Returns kernel function value at given distance.
    CUBBYFLOW_CUDA_HOST_DEVICE float operator()(float distance) const;

    //! Returns the first derivative at given distance.
    CUBBYFLOW_CUDA_HOST_DEVICE float FirstDerivative(float distance) const;

    //! Returns the second derivative at given distance.
    CUBBYFLOW_CUDA_HOST_DEVICE float SecondDerivative(float distance) const;

    //! Returns the gradient at a point.
    CUBBYFLOW_CUDA_HOST_DEVICE float2 Gradient(const float2& point) const;

    //! Returns the gradient at a point defined by distance and direction.
    CUBBYFLOW_CUDA_HOST_DEVICE float2 Gradient(float distance,
                                               const float2& direction) const;

    //! Kernel radius.
    float h;

    //! Square of the kernel radius.
    float h2;

    //! Cubic of the kernel radius.
    float h3;

    //! Forth-power of the kernel radius.
    float h4;
};

//!
//! \brief Spiky 2-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2002 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2002.
//!
struct CUDASPHSpikyKernel2
{
    //! Constructs a kernel object with zero radius.
    CUBBYFLOW_CUDA_HOST_DEVICE CUDASPHSpikyKernel2();

    //! Constructs a kernel object with given radius.
    CUBBYFLOW_CUDA_HOST_DEVICE explicit CUDASPHSpikyKernel2(float kernelRadius);

    //! Returns kernel function value at given distance.
    CUBBYFLOW_CUDA_HOST_DEVICE float operator()(float distance) const;

    //! Returns the first derivative at given distance.
    CUBBYFLOW_CUDA_HOST_DEVICE float FirstDerivative(float distance) const;

    //! Returns the second derivative at given distance.
    CUBBYFLOW_CUDA_HOST_DEVICE float SecondDerivative(float distance) const;

    //! Returns the gradient at a point.
    CUBBYFLOW_CUDA_HOST_DEVICE float2 Gradient(const float2& point) const;

    //! Returns the gradient at a point defined by distance and direction.
    CUBBYFLOW_CUDA_HOST_DEVICE float2 Gradient(float distance,
                                               const float2& direction) const;

    //! Kernel radius.
    float h;

    //! Square of the kernel radius.
    float h2;

    //! Cubic of the kernel radius.
    float h3;

    //! Fourth-power of the kernel radius.
    float h4;

    //! Fifth-power of the kernel radius.
    float h5;
};
}  // namespace CubbyFlow

#include <Core/CUDA/CUDASPHKernels2-Impl.hpp>

#endif

#endif