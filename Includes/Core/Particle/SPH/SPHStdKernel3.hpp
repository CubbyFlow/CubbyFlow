// This code is based on Jet framework.
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

#ifndef CUBBYFLOW_SPH_STD_KERNEL3_HPP
#define CUBBYFLOW_SPH_STD_KERNEL3_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//!
//! \brief Standard 3-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct SPHStdKernel3
{
    //! Constructs a kernel object with zero radius.
    SPHStdKernel3();

    //! Constructs a kernel object with given radius.
    explicit SPHStdKernel3(double kernelRadius);

    //! Returns kernel function value at given distance.
    double operator()(double distance) const;

    //! Returns the first derivative at given distance.
    [[nodiscard]] double FirstDerivative(double distance) const;

    //! Returns the gradient at a point.
    [[nodiscard]] Vector3D Gradient(const Vector3D& point) const;

    //! Returns the gradient at a point defined by distance and direction.
    [[nodiscard]] Vector3D Gradient(double distance,
                                    const Vector3D& direction) const;

    //! Returns the second derivative at given distance.
    [[nodiscard]] double SecondDerivative(double distance) const;

    //! Kernel radius.
    double h;

    //! Square of the kernel radius.
    double h2;

    //! Cubic of the kernel radius.
    double h3;

    //! Fifth-power of the kernel radius.
    double h5;
};

//!
//! \brief Spiky 3-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct SPHSpikyKernel3
{
    //! Constructs a kernel object with zero radius.
    SPHSpikyKernel3();

    //! Constructs a kernel object with given radius.
    explicit SPHSpikyKernel3(double kernelRadius);

    //! Returns kernel function value at given distance.
    double operator()(double distance) const;

    //! Returns the first derivative at given distance.
    [[nodiscard]] double FirstDerivative(double distance) const;

    //! Returns the gradient at a point.
    [[nodiscard]] Vector3D Gradient(const Vector3D& point) const;

    //! Returns the gradient at a point defined by distance and direction.
    [[nodiscard]] Vector3D Gradient(double distance,
                                    const Vector3D& direction) const;

    //! Returns the second derivative at given distance.
    [[nodiscard]] double SecondDerivative(double distance) const;

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
};
}  // namespace CubbyFlow

#endif