// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_SYSTM_DATA_HPP
#define CUBBYFLOW_GRID_SYSTM_DATA_HPP

#include <Core/Grid/FaceCenteredGrid.hpp>
#include <Core/Grid/ScalarGrid.hpp>
#include <Core/Utils/Serialization.hpp>

namespace CubbyFlow
{
//!
//! \brief      N-D grid system data.
//!
//! This class is the key data structure for storing grid system data. To
//! represent a grid system for fluid simulation, velocity field is defined as a
//! face-centered (MAC) grid by default. It can also have additional scalar or
//! vector attributes by adding extra data layer.
//!
template <size_t N>
class GridSystemData : public Serializable
{
 public:
    //! Constructs empty grid system.
    GridSystemData();

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
    GridSystemData(const Vector<size_t, N>& resolution,
                   const Vector<double, N>& gridSpacing,
                   const Vector<double, N>& origin);

    //! Default virtual destructor.
    ~GridSystemData() override = default;

    //! Copy constructor.
    GridSystemData(const GridSystemData& other);

    //! Move constructor.
    GridSystemData(GridSystemData&& other) noexcept;

    //! Copy assignment operator.
    GridSystemData& operator=(const GridSystemData& other);

    //! Move assignment operator.
    GridSystemData& operator=(GridSystemData&& other) noexcept;

    //!
    //! \brief      Resizes the whole system with given resolution, grid
    //!     spacing, and origin.
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
    void Resize(const Vector<size_t, N>& resolution,
                const Vector<double, N>& gridSpacing,
                const Vector<double, N>& origin);

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
    [[nodiscard]] Vector<size_t, N> Resolution() const;

    //! Return the grid spacing.
    [[nodiscard]] Vector<double, N> GridSpacing() const;

    //! Returns the origin of the grid.
    [[nodiscard]] Vector<double, N> Origin() const;

    //! Returns the bounding box of the grid.
    [[nodiscard]] BoundingBox<double, N> GetBoundingBox() const;

    //!
    //! \brief      Adds a non-advectable scalar data grid by passing its
    //!             builder and initial value.
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
    size_t AddScalarData(const std::shared_ptr<ScalarGridBuilder<N>>& builder,
                         double initialVal = 0.0);

    //!
    //! \brief      Adds a non-advectable vector data grid by passing its
    //!             builder and initial value.
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
    size_t AddVectorData(
        const std::shared_ptr<VectorGridBuilder<N>>& builder,
        const Vector<double, N>& initialVal = Vector<double, N>{});

    //!
    //! \brief      Adds an advectable scalar data grid by passing its builder
    //!             and initial value.
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
    size_t AddAdvectableScalarData(
        const std::shared_ptr<ScalarGridBuilder<N>>& builder,
        double initialVal = 0.0);

    //!
    //! \brief      Adds an advectable vector data grid by passing its builder
    //!     and initial value.
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
    size_t AddAdvectableVectorData(
        const std::shared_ptr<VectorGridBuilder<N>>& builder,
        const Vector<double, N>& initialVal = Vector<double, N>{});

    //!
    //! \brief      Returns the velocity field.
    //!
    //! This class has velocity field by default, and it is part of the
    //! advectable vector data list.
    //!
    //! \return     Pointer to the velocity field.
    //!
    [[nodiscard]] const std::shared_ptr<FaceCenteredGrid<N>>& Velocity() const;

    //!
    //! \brief      Returns the index of the velocity field.
    //!
    //! This class has velocity field by default, and it is part of the
    //! advectable vector data list. This function returns the index of the
    //! velocity field from the list.
    //!
    //! \return     Index of the velocity field.
    //!
    [[nodiscard]] size_t VelocityIndex() const;

    //! Returns the non-advectable scalar data at given index.
    [[nodiscard]] const std::shared_ptr<ScalarGrid<N>>& ScalarDataAt(
        size_t idx) const;

    //! Returns the non-advectable vector data at given index.
    [[nodiscard]] const std::shared_ptr<VectorGrid<N>>& VectorDataAt(
        size_t idx) const;

    //! Returns the advectable scalar data at given index.
    [[nodiscard]] const std::shared_ptr<ScalarGrid<N>>& AdvectableScalarDataAt(
        size_t idx) const;

    //! Returns the advectable vector data at given index.
    [[nodiscard]] const std::shared_ptr<VectorGrid<N>>& AdvectableVectorDataAt(
        size_t idx) const;

    //! Returns the number of non-advectable scalar data.
    [[nodiscard]] size_t NumberOfScalarData() const;

    //! Returns the number of non-advectable vector data.
    [[nodiscard]] size_t NumberOfVectorData() const;

    //! Returns the number of advectable scalar data.
    [[nodiscard]] size_t NumberOfAdvectableScalarData() const;

    //! Returns the number of advectable vector data.
    [[nodiscard]] size_t NumberOfAdvectableVectorData() const;

    //! Serialize the data to the given buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Serialize the data from the given buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

 private:
    template <size_t M = N>
    static std::enable_if_t<M == 2, void> Serialize(
        const GridSystemData<2>& grid, std::vector<uint8_t>* buffer);

    template <size_t M = N>
    static std::enable_if_t<M == 3, void> Serialize(
        const GridSystemData<3>& grid, std::vector<uint8_t>* buffer);

    template <size_t M = N>
    static std::enable_if_t<M == 2, void> Deserialize(
        const std::vector<uint8_t>& buffer, GridSystemData<2>& grid);

    template <size_t M = N>
    static std::enable_if_t<M == 3, void> Deserialize(
        const std::vector<uint8_t>& buffer, GridSystemData<3>& grid);

    Vector<size_t, N> m_resolution;
    Vector<double, N> m_gridSpacing;
    Vector<double, N> m_origin;

    std::shared_ptr<FaceCenteredGrid<N>> m_velocity;
    size_t m_velocityIdx = 0;
    std::vector<std::shared_ptr<ScalarGrid<N>>> m_scalarDataList;
    std::vector<std::shared_ptr<VectorGrid<N>>> m_vectorDataList;
    std::vector<std::shared_ptr<ScalarGrid<N>>> m_advectableScalarDataList;
    std::vector<std::shared_ptr<VectorGrid<N>>> m_advectableVectorDataList;
};

//! 2-D GridSystemData type.
using GridSystemData2 = GridSystemData<2>;

//! 3-D GridSystemData type.
using GridSystemData3 = GridSystemData<3>;

//! Shared pointer type of GridSystemData2.
using GridSystemData2Ptr = std::shared_ptr<GridSystemData2>;

//! Shared pointer type of GridSystemData3.
using GridSystemData3Ptr = std::shared_ptr<GridSystemData3>;
}  // namespace CubbyFlow

#endif