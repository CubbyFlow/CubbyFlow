// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR_FIELD2_HPP
#define CUBBYFLOW_VECTOR_FIELD2_HPP

#include <Core/Field/Field2.hpp>
#include <Core/Vector/Vector2.hpp>

#include <functional>
#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 2-D vector field.
class VectorField2 : public Field2
{
 public:
    //! Default constructor.
    VectorField2();

    //! Default destructor.
    virtual ~VectorField2();

    //! Returns sampled value at given position \p x.
    virtual Vector2D Sample(const Vector2D& x) const = 0;

    //! Returns divergence at given position \p x.
    virtual double Divergence(const Vector2D& x) const;

    //! Returns curl at given position \p x.
    virtual double Curl(const Vector2D& x) const;

    //! Returns sampler function object.
    virtual std::function<Vector2D(const Vector2D&)> Sampler() const;
};

//! Shared pointer for the VectorField2 type.
using VectorField2Ptr = std::shared_ptr<VectorField2>;
}  // namespace CubbyFlow

#endif