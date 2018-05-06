/*************************************************************************
> File Name: BccLatticPointGenerator.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Body-centered lattice points generator.
> Created Time: 2017/06/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_BCC_LATTICE_POINT_GENERATOR_H
#define CUBBYFLOW_BCC_LATTICE_POINT_GENERATOR_H

#include <Core/PointGenerator/PointGenerator3.h>

namespace CubbyFlow
{
	//!
	//! \brief Body-centered lattice points generator.
	//!
	//! \see http://en.wikipedia.org/wiki/Cubic_crystal_system
	//!      http://mathworld.wolfram.com/CubicClosePacking.html
	//!
	class BccLatticePointGenerator final : public PointGenerator3
	{
	public:
		//!
		//! \brief Invokes \p callback function for each BCC-lattice points inside
		//! \p boundingBox.
		//!
		//! This function iterates every BCC-lattice points inside \p boundingBox
		//! where \p spacing is the size of the unit cell of BCC structure.
		//!
		void ForEachPoint(const BoundingBox3D& boundingBox, double spacing,
			const std::function<bool(const Vector3D&)>& callback) const override;
	};

	//! Shared pointer type for the BccLatticePointGenerator.
	using BccLatticePointGeneratorPtr = std::shared_ptr<BccLatticePointGenerator>;
}

#endif