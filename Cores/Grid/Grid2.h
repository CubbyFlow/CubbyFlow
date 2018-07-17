/*************************************************************************
> File Name: Grid2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D cartesian grid structure.
> Created Time: 2017/07/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_GRID2_H
#define CUBBYFLOW_GRID2_H

#include <BoundingBox/BoundingBox2.h>
#include <Size/Size2.h>
#include <Utils/Serialization.h>
#include <Vector/Vector2.h>

#include <memory>
#include <string>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for 2-D cartesian grid structure.
	//!
	//! This class represents 2-D cartesian grid structure. This class is an
	//! abstract base class and does not store any data. The class only stores the
	//! shape of the grid. The grid structure is axis-aligned and can have different
	//! grid spacing per axis.
	//!
	class Grid2 : public Serializable
	{
	public:
		//! Function type for mapping data index to actual position.
		using DataPositionFunc = std::function<Vector2D(size_t, size_t)>;

		//! Constructs an empty grid.
		Grid2();

		//! Default destructor.
		virtual ~Grid2();

		//! Returns the type name of derived grid.
		virtual std::string TypeName() const = 0;

		//! Returns the grid resolution.
		const Size2& Resolution() const;

		//! Returns the grid origin.
		const Vector2D& Origin() const;

		//! Returns the grid spacing.
		const Vector2D& GridSpacing() const;

		//! Returns the bounding box of the grid.
		const BoundingBox2D& BoundingBox() const;

		//! Returns the function that maps grid index to the cell-center position.
		DataPositionFunc CellCenterPosition() const;

		//!
		//! \brief Invokes the given function \p func for each grid cell.
		//!
		//! This function invokes the given function object \p func for each grid
		//! cell in serial manner. The input parameters are i and j indices of a
		//! grid cell. The order of execution is i-first, j-last.
		//!
		void ForEachCellIndex(const std::function<void(size_t, size_t)>& func) const;

		//!
		//! \brief Invokes the given function \p func for each grid cell in parallel.
		//!
		//! This function invokes the given function object \p func for each grid
		//! cell in parallel manner. The input parameters are i and j indices of a
		//! grid cell. The order of execution can be arbitrary since it's
		//! multi-threaded.
		//!
		void ParallelForEachCellIndex(const std::function<void(size_t, size_t)>& func) const;

		//! Returns true if resolution, grid-spacing and origin are same.
		bool HasSameShape(const Grid2& other) const;

		//! Swaps the data with other grid.
		virtual void Swap(Grid2* other) = 0;

	protected:
		//! Sets the size parameters including the resolution, grid spacing, and origin.
		void SetSizeParameters(const Size2& resolution, const Vector2D& gridSpacing, const Vector2D& origin);

		//! Swaps the size parameters with given grid \p other.
		void SwapGrid(Grid2* other);

		//! Sets the size parameters with given grid \p other.
		void SetGrid(const Grid2& other);

		//! Fetches the data into a continuous linear array.
		virtual void GetData(std::vector<double>* data) const = 0;

		//! Sets the data from a continuous linear array.
		virtual void SetData(const std::vector<double>& data) = 0;

	private:
		Size2 m_resolution;
		Vector2D m_gridSpacing = Vector2D(1, 1);
		Vector2D m_origin;
		BoundingBox2D m_boundingBox = BoundingBox2D(Vector2D(), Vector2D());
	};

	using Grid2Ptr = std::shared_ptr<Grid2>;

#define CUBBYFLOW_GRID2_TYPE_NAME(DerivedClassName) \
	std::string TypeName() const override \
	{ \
		return #DerivedClassName; \
	}
}

#endif