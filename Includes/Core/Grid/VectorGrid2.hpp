// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR_GRID2_HPP
#define CUBBYFLOW_VECTOR_GRID2_HPP

#include <Core/Array/ArrayAccessor2.hpp>
#include <Core/Field/VectorField2.hpp>
#include <Core/Grid/Grid2.hpp>

namespace CubbyFlow
{
//! Abstract base class for 2-D vector grid structure.
class VectorGrid2 : public VectorField2, public Grid2
{
 public:
    //! Read-write array accessor type.
    using VectorDataAccessor = ArrayAccessor2<Vector2D>;

    //! Read-only array accessor type.
    using ConstVectorDataAccessor = ConstArrayAccessor2<Vector2D>;

    //! Constructs an empty grid.
    VectorGrid2() = default;

    //! Default copy constructor.
    VectorGrid2(const VectorGrid2&) = default;

    //! Default move constructor.
    VectorGrid2(VectorGrid2&&) noexcept = default;

    //! Default virtual destructor.
    ~VectorGrid2() override = default;

    //! Default copy assignment operator.
    VectorGrid2& operator=(const VectorGrid2&) = default;

    //! Default move assignment operator.
    VectorGrid2& operator=(VectorGrid2&&) noexcept = default;

    //! Clears the contents of the grid.
    void Clear();

    //! Resizes the grid using given parameters.
    void Resize(size_t resolutionX, size_t resolutionY,
                double gridSpacingX = 1.0, double gridSpacingY = 1.0,
                double originX = 0.0, double originY = 0.0,
                double initialValueX = 0.0, double initialValueY = 0.0);

    //! Resizes the grid using given parameters.
    void Resize(const Size2& resolution,
                const Vector2D& gridSpacing = Vector2D{ 1, 1 },
                const Vector2D& origin = Vector2D{},
                const Vector2D& initialValue = Vector2D{});

    //! Resizes the grid using given parameters.
    void Resize(double gridSpacingX, double gridSpacingY, double originX,
                double originY);

    //! Resizes the grid using given parameters.
    void Resize(const Vector2D& gridSpacing, const Vector2D& origin);

    //! Fills the grid with given value.
    virtual void Fill(const Vector2D& value,
                      ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

    //! Fills the grid with given position-to-value mapping function.
    virtual void Fill(const std::function<Vector2D(const Vector2D&)>& func,
                      ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

    //! Returns the copy of the grid instance.
    [[nodiscard]] virtual std::shared_ptr<VectorGrid2> Clone() const = 0;

    //! Serializes the grid instance to the output buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes the input buffer to the grid instance.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

 protected:
    //!
    //! \brief Invoked when the resizing happens.
    //!
    //! This callback function is called when the grid gets resized. The
    //! overriding class should allocate the internal storage based on its
    //! data layout scheme.
    //!
    virtual void OnResize(const Size2& resolution, const Vector2D& gridSpacing,
                          const Vector2D& origin,
                          const Vector2D& initialValue) = 0;
};

//! Shared pointer for the VectorGrid2 type.
using VectorGrid2Ptr = std::shared_ptr<VectorGrid2>;

//! Abstract base class for 2-D vector grid builder.
class VectorGridBuilder2
{
 public:
    //! Default constructor.
    VectorGridBuilder2() = default;

    //! Default copy constructor.
    VectorGridBuilder2(const VectorGridBuilder2&) = default;

    //! Default move constructor.
    VectorGridBuilder2(VectorGridBuilder2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~VectorGridBuilder2() = default;

    //! Default copy assignment operator.
    VectorGridBuilder2& operator=(const VectorGridBuilder2&) = default;

    //! Default move assignment operator.
    VectorGridBuilder2& operator=(VectorGridBuilder2&&) noexcept = default;

    //! Returns 2-D vector grid with given parameters.
    [[nodiscard]] virtual VectorGrid2Ptr Build(
        const Size2& resolution, const Vector2D& gridSpacing,
        const Vector2D& gridOrigin, const Vector2D& initialVal) const = 0;
};

//! Shared pointer for the VectorGridBuilder2 type.
using VectorGridBuilder2Ptr = std::shared_ptr<VectorGridBuilder2>;
}  // namespace CubbyFlow

#endif