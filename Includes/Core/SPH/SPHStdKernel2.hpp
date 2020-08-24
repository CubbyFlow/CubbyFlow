﻿// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Adopted from the sample code of:
// Bart Adams and Martin Wicke,
// "Meshless Approximation Methods and Applications in Physics Based Modeling
// and Animation", Eurographics 2009 Tutorial

#ifndef CUBBYFLOW_SPH_STD_KERNEL2_HPP
#define CUBBYFLOW_SPH_STD_KERNEL2_HPP

#include <Core/Vector/Vector2.hpp>

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
struct SPHStdKernel2
{
    //! Kernel radius.
    double h;

    //! Square of the kernel radius.
    double h2;

    //! Cubic of the kernel radius.
    double h3;

    //! Fourth-power of the kernel radius.
    double h4;

    //! Constructs a kernel object with zero radius.
    SPHStdKernel2();

    //! Constructs a kernel object with given radius.
    explicit SPHStdKernel2(double kernelRadius);

    //! Copy constructor.
    SPHStdKernel2(const SPHStdKernel2& other);

    //! Default copy assignment operator.
    SPHStdKernel2& operator=(const SPHStdKernel2& other) = default;

    //! Returns kernel function value at given distance.
    double operator()(double distance) const;

    //! Returns the first derivative at given distance.
    double FirstDerivative(double distance) const;

    //! Returns the gradient at a point.
    Vector2D Gradient(const Vector2D& point) const;

    //! Returns the gradient at a point defined by distance and direction.
    Vector2D Gradient(double distance, const Vector2D& direction) const;

    //! Returns the second derivative at given distance.
    double SecondDerivative(double distance) const;
};

//!
//! \brief Spiky 2-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct SPHSpikyKernel2
{
    //! Kernel radius.
    double h;

    //! Square of the kernel radius.
    double h2;

    //! Cubic of the kernel radius.
    double h3;

    //! Fourth-power of the kernel radius.
    double h4;

    //! Fifth-power of the kernel radius.
    double h5;

    //! Constructs a kernel object with zero radius.
    SPHSpikyKernel2();

    //! Constructs a kernel object with given radius.
    explicit SPHSpikyKernel2(double kernelRadius);

    //! Copy constructor.
    SPHSpikyKernel2(const SPHSpikyKernel2& other);

    //! Default copy assignment operator.
    SPHSpikyKernel2& operator=(const SPHSpikyKernel2& other) = default;

    //! Returns kernel function value at given distance.
    double operator()(double distance) const;

    //! Returns the first derivative at given distance.
    double FirstDerivative(double distance) const;

    //! Returns the gradient at a point.
    Vector2D Gradient(const Vector2D& point) const;

    //! Returns the gradient at a point defined by distance and direction.
    Vector2D Gradient(double distance, const Vector2D& direction) const;

    //! Returns the second derivative at given distance.
    double SecondDerivative(double distance) const;
};
}  // namespace CubbyFlow

#endif