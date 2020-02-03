// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SCALAR_FIELD2_HPP
#define CUBBYFLOW_SCALAR_FIELD2_HPP

#include <Core/Vector/Vector2.h>
#include <Core/Field/Field2.hpp>

#include <functional>
#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 2-D scalar field.
class ScalarField2 : public Field2
{
 public:
    //! Default constructor.
    ScalarField2();

    //! Default destructor.
    virtual ~ScalarField2();

    //! Returns sampled value at given position \p x.
    virtual double Sample(const Vector2D& x) const = 0;

    //! Returns gradient vector at given position \p x.
    virtual Vector2D Gradient(const Vector2D& x) const;

    //! Returns Laplacian at given position \p x.
    virtual double Laplacian(const Vector2D& x) const;

    //! Returns sampler function object.
    virtual std::function<double(const Vector2D&)> Sampler() const;
};

//! Shared pointer for the ScalarField2 type.
using ScalarField2Ptr = std::shared_ptr<ScalarField2>;
}  // namespace CubbyFlow

#endif