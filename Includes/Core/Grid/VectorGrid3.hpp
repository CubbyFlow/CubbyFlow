// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR_GRID3_HPP
#define CUBBYFLOW_VECTOR_GRID3_HPP

#include <Core/Array/ArrayView.hpp>
#include <Core/Field/VectorField.hpp>
#include <Core/Grid/Grid.hpp>

namespace CubbyFlow
{
//! Abstract base class for 3-D vector grid structure.
class VectorGrid3 : public VectorField3, public Grid3
{
 public:
    //! Read-write array view type.
    using VectorDataView = ArrayView3<Vector3D>;

    //! Read-only array view type.
    using ConstVectorDataView = ConstArrayView3<Vector3D>;

    //! Constructs an empty grid.
    VectorGrid3() = default;

    //! Default copy constructor.
    VectorGrid3(const VectorGrid3&) = default;

    //! Default move constructor.
    VectorGrid3(VectorGrid3&&) noexcept = default;

    //! Default virtual destructor.
    ~VectorGrid3() override = default;

    //! Default copy assignment operator.
    VectorGrid3& operator=(const VectorGrid3&) = default;

    //! Default move assignment operator.
    VectorGrid3& operator=(VectorGrid3&&) noexcept = default;

    //! Clears the contents of the grid.
    void Clear();

    //! Resizes the grid using given parameters.
    void Resize(size_t resolutionX, size_t resolutionY, size_t resolutionZ,
                double gridSpacingX = 1.0, double gridSpacingY = 1.0,
                double gridSpacingZ = 1.0, double originX = 0.0,
                double originY = 0.0, double originZ = 0.0,
                double initialValueX = 0.0, double initialValueY = 0.0,
                double initialValueZ = 0.0);

    //! Resizes the grid using given parameters.
    void Resize(const Vector3UZ& resolution,
                const Vector3D& gridSpacing = Vector3D{ 1, 1, 1 },
                const Vector3D& origin = Vector3D{},
                const Vector3D& initialValue = Vector3D{});

    //! Resizes the grid using given parameters.
    void Resize(double gridSpacingX, double gridSpacingY, double gridSpacingZ,
                double originX, double originY, double originZ);

    //! Resizes the grid using given parameters.
    void Resize(const Vector3D& gridSpacing, const Vector3D& origin);

    //! Fills the grid with given value.
    virtual void Fill(const Vector3D& value,
                      ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

    //! Fills the grid with given position-to-value mapping function.
    virtual void Fill(const std::function<Vector3D(const Vector3D&)>& func,
                      ExecutionPolicy policy = ExecutionPolicy::Parallel) = 0;

    //! Returns the copy of the grid instance.
    [[nodiscard]] virtual std::shared_ptr<VectorGrid3> Clone() const = 0;

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
    virtual void OnResize(const Vector3UZ& resolution,
                          const Vector3D& gridSpacing, const Vector3D& origin,
                          const Vector3D& initialValue) = 0;
};

//! Shared pointer for the VectorGrid3 type.
using VectorGrid3Ptr = std::shared_ptr<VectorGrid3>;

//! Abstract base class for 3-D vector grid builder.
class VectorGridBuilder3
{
 public:
    //! Default constructor.
    VectorGridBuilder3() = default;

    //! Default copy constructor.
    VectorGridBuilder3(const VectorGridBuilder3&) = default;

    //! Default move constructor.
    VectorGridBuilder3(VectorGridBuilder3&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~VectorGridBuilder3() = default;

    //! Default copy assignment operator.
    VectorGridBuilder3& operator=(const VectorGridBuilder3&) = default;

    //! Default move assignment operator.
    VectorGridBuilder3& operator=(VectorGridBuilder3&&) noexcept = default;

    //! Returns 3-D vector grid with given parameters.
    [[nodiscard]] virtual VectorGrid3Ptr Build(
        const Vector3UZ& resolution, const Vector3D& gridSpacing,
        const Vector3D& gridOrigin, const Vector3D& initialVal) const = 0;
};

//! Shared pointer for the VectorGridBuilder3 type.
using VectorGridBuilder3Ptr = std::shared_ptr<VectorGridBuilder3>;
}  // namespace CubbyFlow

#endif