// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SCALAR_FIELD3_HPP
#define CUBBYFLOW_SCALAR_FIELD3_HPP

#include <Core/Field/Field3.hpp>
#include <Core/Vector/Vector3.hpp>

#include <functional>
#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 3-D scalar field.
class ScalarField3 : public Field3
{
 public:
    //! Default constructor.
    ScalarField3();

    //! Default destructor.
    virtual ~ScalarField3();

    //! Returns sampled value at given position \p x.
    virtual double Sample(const Vector3D& x) const = 0;

    //! Returns gradient vector at given position \p x.
    virtual Vector3D Gradient(const Vector3D& x) const;

    //! Returns Laplacian at given position \p x.
    virtual double Laplacian(const Vector3D& x) const;

    //! Returns sampler function object.
    virtual std::function<double(const Vector3D&)> Sampler() const;
};

//! Shared pointer for the ScalarField3 type.
using ScalarField3Ptr = std::shared_ptr<ScalarField3>;
}  // namespace CubbyFlow

#endif