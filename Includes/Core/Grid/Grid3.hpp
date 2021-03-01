// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID3_HPP
#define CUBBYFLOW_GRID3_HPP

#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Matrix/Matrix.hpp>
#include <Core/Utils/Serialization.hpp>

#include <memory>
#include <string>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 3-D cartesian grid structure.
//!
//! This class represents 3-D cartesian grid structure. This class is an
//! abstract base class and does not store any data. The class only stores the
//! shape of the grid. The grid structure is axis-aligned and can have different
//! grid spacing per axis.
//!
class Grid3 : public Serializable
{
 public:
    //! Function type for mapping data index to actual position.
    using DataPositionFunc = std::function<Vector3D(size_t, size_t, size_t)>;

    //! Default constructor.
    Grid3() = default;

    //! Default copy constructor.
    Grid3(const Grid3&) = default;

    //! Default move constructor.
    Grid3(Grid3&&) noexcept = default;

    //! Default virtual destructor.
    ~Grid3() override = default;

    //! Default copy assignment operator.
    Grid3& operator=(const Grid3&) = default;

    //! Default move assignment operator.
    Grid3& operator=(Grid3&&) noexcept = default;

    //! Returns the type name of derived grid.
    [[nodiscard]] virtual std::string TypeName() const = 0;

    //! Returns the grid resolution.
    [[nodiscard]] const Vector3UZ& Resolution() const;

    //! Returns the grid origin.
    [[nodiscard]] const Vector3D& Origin() const;

    //! Returns the grid spacing.
    [[nodiscard]] const Vector3D& GridSpacing() const;

    //! Returns the bounding box of the grid.
    [[nodiscard]] const BoundingBox3D& BoundingBox() const;

    //! Returns the function that maps grid index to the cell-center position.
    [[nodiscard]] DataPositionFunc CellCenterPosition() const;

    //!
    //! \brief Invokes the given function \p func for each grid cell.
    //!
    //! This function invokes the given function object \p func for each grid
    //! cell in serial manner. The input parameters are i, j and k indices of a
    //! grid cell. The order of execution is i-first, j-next, k-last.
    //!
    void ForEachCellIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;

    //!
    //! \brief Invokes the given function \p func for each grid cell in
    //! parallel.
    //!
    //! This function invokes the given function object \p func for each grid
    //! cell in parallel manner. The input parameters are i, j and k indices of
    //! a grid cell. The order of execution can be arbitrary since it's
    //! multi-threaded.
    //!
    void ParallelForEachCellIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;

    //! Serializes the grid instance to the output buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override = 0;

    //! Deserializes the input buffer to the grid instance.
    void Deserialize(const std::vector<uint8_t>& buffer) override = 0;

    //! Returns true if resolution, grid-spacing and origin are same.
    [[nodiscard]] bool HasSameShape(const Grid3& other) const;

    //! Swaps the data with other grid.
    virtual void Swap(Grid3* other) = 0;

 protected:
    //! Sets the size parameters including the resolution, grid spacing, and
    //! origin.
    void SetSizeParameters(const Vector3UZ& resolution,
                           const Vector3D& gridSpacing, const Vector3D& origin);

    //! Swaps the size parameters with given grid \p other.
    void SwapGrid(Grid3* other);

    //! Sets the size parameters with given grid \p other.
    void SetGrid(const Grid3& other);

    //! Fetches the data into a continuous linear array.
    virtual void GetData(std::vector<double>* data) const = 0;

    //! Sets the data from a continuous linear array.
    virtual void SetData(const std::vector<double>& data) = 0;

 private:
    Vector3UZ m_resolution;
    Vector3D m_gridSpacing = Vector3D(1, 1, 1);
    Vector3D m_origin;
    BoundingBox3D m_boundingBox = BoundingBox3D(Vector3D(), Vector3D());
};

using Grid3Ptr = std::shared_ptr<Grid3>;

#define CUBBYFLOW_GRID3_TYPE_NAME(DerivedClassName) \
    std::string TypeName() const override           \
    {                                               \
        return #DerivedClassName;                   \
    }

}  // namespace CubbyFlow

#endif