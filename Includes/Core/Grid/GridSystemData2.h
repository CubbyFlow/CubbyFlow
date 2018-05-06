/*************************************************************************
> File Name: GridSystemData2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D grid system data.
> Created Time: 2017/08/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID_SYSTM_DATA2_H
#define CUBBYFLOW_GRID_SYSTM_DATA2_H

#include <Core/Grid/FaceCenteredGrid2.h>
#include <Core/Grid/ScalarGrid2.h>
#include <Core/Utils/Serialization.h>

namespace CubbyFlow
{
	//!
	//! \brief      2-D grid system data.
	//!
	//! This class is the key data structure for storing grid system data. To
	//! represent a grid system for fluid simulation, velocity field is defined as a
	//! face-centered (MAC) grid by default. It can also have additional scalar or
	//! vector attributes by adding extra data layer.
	//!
	class GridSystemData2 : public Serializable
	{
	public:
		//! Constructs empty grid system.
		GridSystemData2();

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
		GridSystemData2(const Size2& resolution, const Vector2D& gridSpacing, const Vector2D& origin);

		//! Copy constructor.
		GridSystemData2(const GridSystemData2& other);

		//! Destructor.
		virtual ~GridSystemData2();

		//!
		//! \brief      Resizes the whole system with given resolution, grid
		//!				spacing, and origin.
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
		void Resize(const Size2& resolution, const Vector2D& gridSpacing, const Vector2D& origin);

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
		Size2 GetResolution() const;

		//! Return the grid spacing.
		Vector2D GetGridSpacing() const;

		//! Returns the origin of the grid.
		Vector2D GetOrigin() const;

		//! Returns the bounding box of the grid.
		BoundingBox2D GetBoundingBox() const;

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
		size_t AddScalarData(const ScalarGridBuilder2Ptr& builder, double initialVal = 0.0);

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
		size_t AddVectorData(const VectorGridBuilder2Ptr& builder, const Vector2D& initialVal = Vector2D());

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
		size_t AddAdvectableScalarData(const ScalarGridBuilder2Ptr& builder, double initialVal = 0.0);

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
		size_t AddAdvectableVectorData(const VectorGridBuilder2Ptr& builder, const Vector2D& initialVal = Vector2D());

		//!
		//! \brief      Returns the velocity field.
		//!
		//! This class has velocity field by default, and it is part of the
		//! advectable vector data list.
		//!
		//! \return     Pointer to the velocity field.
		//!
		const FaceCenteredGrid2Ptr& GetVelocity() const;

		//!
		//! \brief      Returns the index of the velocity field.
		//!
		//! This class has velocity field by default, and it is part of the
		//! advectable vector data list. This function returns the index of the
		//! velocity field from the list.
		//!
		//! \return     Index of the velocity field.
		//!
		size_t GetVelocityIndex() const;

		//! Returns the non-advectable scalar data at given index.
		const ScalarGrid2Ptr& GetScalarDataAt(size_t idx) const;

		//! Returns the non-advectable vector data at given index.
		const VectorGrid2Ptr& GetVectorDataAt(size_t idx) const;

		//! Returns the advectable scalar data at given index.
		const ScalarGrid2Ptr& GetAdvectableScalarDataAt(size_t idx) const;

		//! Returns the advectable vector data at given index.
		const VectorGrid2Ptr& GetAdvectableVectorDataAt(size_t idx) const;

		//! Returns the number of non-advectable scalar data.
		size_t GetNumberOfScalarData() const;

		//! Returns the number of non-advectable vector data.
		size_t GetNumberOfVectorData() const;

		//! Returns the number of advectable scalar data.
		size_t GetNumberOfAdvectableScalarData() const;

		//! Returns the number of advectable vector data.
		size_t GetNumberOfAdvectableVectorData() const;

		//! Serialize the data to the given buffer.
		void Serialize(std::vector<uint8_t>* buffer) const override;

		//! Serialize the data from the given buffer.
		void Deserialize(const std::vector<uint8_t>& buffer) override;

	private:
		Size2 m_resolution;
		Vector2D m_gridSpacing;
		Vector2D m_origin;

		FaceCenteredGrid2Ptr m_velocity;
		size_t m_velocityIdx;
		std::vector<ScalarGrid2Ptr> m_scalarDataList;
		std::vector<VectorGrid2Ptr> m_vectorDataList;
		std::vector<ScalarGrid2Ptr> m_advectableScalarDataList;
		std::vector<VectorGrid2Ptr> m_advectableVectorDataList;
	};

	//! Shared pointer type of GridSystemData2.
	using GridSystemData2Ptr = std::shared_ptr<GridSystemData2>;
}

#endif