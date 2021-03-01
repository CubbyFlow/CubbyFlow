// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_SYSTM_DATA3_HPP
#define CUBBYFLOW_GRID_SYSTM_DATA3_HPP

#include <Core/Grid/FaceCenteredGrid3.hpp>
#include <Core/Grid/ScalarGrid3.hpp>
#include <Core/Utils/Serialization.hpp>

namespace CubbyFlow
{
//!
//! \brief      3-D grid system data.
//!
//! This class is the key data structure for storing grid system data. To
//! represent a grid system for fluid simulation, velocity field is defined as a
//! face-centered (MAC) grid by default. It can also have additional scalar or
//! vector attributes by adding extra data layer.
//!
class GridSystemData3 : public Serializable
{
 public:
    //! Constructs empty grid system.
    GridSystemData3();

    //!
    //! \brief      Constructs a grid system with given resolution, grid spacing
    //!             and origin.
    //!
    //! This constructor builds the entire grid layers within the system. Note,
    //! the resolution is the grid resolution, not the data size of each grid.
    //! Depending on the layout of the grid, the data point may lie on different
    //! part of the grid (vertex, cell-center, or face-center), thus can have
    //! different array size internally. The resolution of the grid means the
    //! grid cell resolution.
    //!
    //! \param[in]  resolution  The resolution.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  origin      The origin.
    //!
    GridSystemData3(const Vector3UZ& resolution, const Vector3D& gridSpacing,
                    const Vector3D& origin);

    //! Copy constructor.
    GridSystemData3(const GridSystemData3& other);

    //! Deleted move constructor.
    GridSystemData3(GridSystemData3&& other) noexcept = delete;

    //! Default virtual destructor.
    ~GridSystemData3() override = default;

    //! Copy assignment operator.
    GridSystemData3& operator=(const GridSystemData3& other);

    //! Deleted move assignment operator.
    GridSystemData3& operator=(GridSystemData3&& other) noexcept = delete;

    //!
    //! \brief      Resizes the whole system with given resolution, grid
    //!             spacing, and origin.
    //!
    //! This function resizes the entire grid layers within the system. Note,
    //! the resolution is the grid resolution, not the data size of each grid.
    //! Depending on the layout of the grid, the data point may lie on different
    //! part of the grid (vertex, cell-center, or face-center), thus can have
    //! different array size internally. The resolution of the grid means the
    //! grid cell resolution.
    //!
    //! \param[in]  resolution  The resolution.
    //! \param[in]  gridSpacing The grid spacing.
    //! \param[in]  origin      The origin.
    //!
    void Resize(const Vector3UZ& resolution, const Vector3D& gridSpacing,
                const Vector3D& origin);

    //!
    //! \brief      Returns the resolution of the grid.
    //!
    //! This function resizes the entire grid layers within the system. Note,
    //! the resolution is the grid resolution, not the data size of each grid.
    //! Depending on the layout of the grid, the data point may lie on different
    //! part of the grid (vertex, cell-center, or face-center), thus can have
    //! different array size internally. The resolution of the grid means the
    //! grid cell resolution.
    //!
    //! \return     Grid cell resolution.
    //!
    [[nodiscard]] Vector3UZ GetResolution() const;

    //! Return the grid spacing.
    [[nodiscard]] Vector3D GetGridSpacing() const;

    //! Returns the origin of the grid.
    [[nodiscard]] Vector3D GetOrigin() const;

    //! Returns the bounding box of the grid.
    [[nodiscard]] BoundingBox3D GetBoundingBox() const;

    //!
    //! \brief      Adds a non-advectable scalar data grid by passing its
    //!				builder and initial value.
    //!
    //! This function adds a new scalar data grid. This layer is not advectable,
    //! meaning that during the computation of fluid flow, this layer won't
    //! follow the flow. For the future access of this layer, its index is
    //! returned.
    //!
    //! \param[in]  builder    The grid builder.
    //! \param[in]  initialVal The initial value.
    //!
    //! \return     Index of the data.
    //!
    [[nodiscard]] size_t AddScalarData(const ScalarGridBuilder3Ptr& builder,
                                       double initialVal = 0.0);

    //!
    //! \brief      Adds a non-advectable vector data grid by passing its
    //!				builder and initial value.
    //!
    //! This function adds a new vector data grid. This layer is not advectable,
    //! meaning that during the computation of fluid flow, this layer won't
    //! follow the flow. For the future access of this layer, its index is
    //! returned.
    //!
    //! \param[in]  builder    The grid builder.
    //! \param[in]  initialVal The initial value.
    //!
    //! \return     Index of the data.
    //!
    [[nodiscard]] size_t AddVectorData(const VectorGridBuilder3Ptr& builder,
                                       const Vector3D& initialVal = Vector3D());

    //!
    //! \brief      Adds an advectable scalar data grid by passing its builder
    //!				and initial value.
    //!
    //! This function adds a new scalar data grid. This layer is advectable,
    //! meaning that during the computation of fluid flow, this layer will
    //! follow the flow. For the future access of this layer, its index is
    //! returned.
    //!
    //! \param[in]  builder    The grid builder.
    //! \param[in]  initialVal The initial value.
    //!
    //! \return     Index of the data.
    //!
    [[nodiscard]] size_t AddAdvectableScalarData(
        const ScalarGridBuilder3Ptr& builder, double initialVal = 0.0);

    //!
    //! \brief      Adds an advectable vector data grid by passing its builder
    //!				and initial value.
    //!
    //! This function adds a new vector data grid. This layer is advectable,
    //! meaning that during the computation of fluid flow, this layer will
    //! follow the flow. For the future access of this layer, its index is
    //! returned.
    //!
    //! \param[in]  builder    The grid builder.
    //! \param[in]  initialVal The initial value.
    //!
    //! \return     Index of the data.
    //!
    [[nodiscard]] size_t AddAdvectableVectorData(
        const VectorGridBuilder3Ptr& builder,
        const Vector3D& initialVal = Vector3D());

    //!
    //! \brief      Returns the velocity field.
    //!
    //! This class has velocity field by default, and it is part of the
    //! advectable vector data list.
    //!
    //! \return     Pointer to the velocity field.
    //!
    [[nodiscard]] const FaceCenteredGrid3Ptr& GetVelocity() const;

    //!
    //! \brief      Returns the index of the velocity field.
    //!
    //! This class has velocity field by default, and it is part of the
    //! advectable vector data list. This function returns the index of the
    //! velocity field from the list.
    //!
    //! \return     Index of the velocity field.
    //!
    [[nodiscard]] size_t GetVelocityIndex() const;

    //! Returns the non-advectable scalar data at given index.
    [[nodiscard]] const ScalarGrid3Ptr& GetScalarDataAt(size_t idx) const;

    //! Returns the non-advectable vector data at given index.
    [[nodiscard]] const VectorGrid3Ptr& GetVectorDataAt(size_t idx) const;

    //! Returns the advectable scalar data at given index.
    [[nodiscard]] const ScalarGrid3Ptr& GetAdvectableScalarDataAt(
        size_t idx) const;

    //! Returns the advectable vector data at given index.
    [[nodiscard]] const VectorGrid3Ptr& GetAdvectableVectorDataAt(
        size_t idx) const;

    //! Returns the number of non-advectable scalar data.
    [[nodiscard]] size_t GetNumberOfScalarData() const;

    //! Returns the number of non-advectable vector data.
    [[nodiscard]] size_t GetNumberOfVectorData() const;

    //! Returns the number of advectable scalar data.
    [[nodiscard]] size_t GetNumberOfAdvectableScalarData() const;

    //! Returns the number of advectable vector data.
    [[nodiscard]] size_t GetNumberOfAdvectableVectorData() const;

    //! Serialize the data to the given buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Serialize the data from the given buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

 private:
    Vector3UZ m_resolution;
    Vector3D m_gridSpacing;
    Vector3D m_origin;

    FaceCenteredGrid3Ptr m_velocity;
    size_t m_velocityIdx;
    std::vector<ScalarGrid3Ptr> m_scalarDataList;
    std::vector<VectorGrid3Ptr> m_vectorDataList;
    std::vector<ScalarGrid3Ptr> m_advectableScalarDataList;
    std::vector<VectorGrid3Ptr> m_advectableVectorDataList;
};

//! Shared pointer type of GridSystemData3.
using GridSystemData3Ptr = std::shared_ptr<GridSystemData3>;
}  // namespace CubbyFlow

#endif