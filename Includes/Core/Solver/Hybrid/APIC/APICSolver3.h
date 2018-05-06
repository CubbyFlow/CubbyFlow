/*************************************************************************
> File Name: APICSolver3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D Affine Particle-in-Cell (APIC) implementation.
> Created Time: 2017/10/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_APIC_SOLVER3_H
#define CUBBYFLOW_APIC_SOLVER3_H

#include <Core/Solver/Hybrid/PIC/PICSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D Affine Particle-in-Cell (APIC) implementation
	//!
	//! This class implements 3-D Affine Particle-in-Cell (APIC) solver from the
	//! SIGGRAPH paper, Jiang 2015.
	//!
	//! \see Jiang, Chenfanfu, et al. "The affine particle-in-cell method."
	//!      ACM Transactions on Graphics (TOG) 34.4 (2015): 51.
	//!
	class APICSolver3 : public PICSolver3
	{
	public:
		class Builder;

		//! Default constructor.
		APICSolver3();

		//! Constructs solver with initial grid size.
		APICSolver3(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin);

		//! Default destructor.
		virtual ~APICSolver3();

		//! Returns builder fox APICSolver3.
		static Builder GetBuilder();

	protected:
		//! Transfers velocity field from particles to grids.
		void TransferFromParticlesToGrids() override;

		//! Transfers velocity field from grids to particles.
		void TransferFromGridsToParticles() override;

	private:
		Array1<Vector3D> m_cX;
		Array1<Vector3D> m_cY;
		Array1<Vector3D> m_cZ;
	};

	//! Shared pointer type for the APICSolver3.
	using APICSolver3Ptr = std::shared_ptr<APICSolver3>;

	//!
	//! \brief Front-end to create APICSolver3 objects step by step.
	//!
	class APICSolver3::Builder final : public GridFluidSolverBuilderBase3<APICSolver3::Builder>
	{
	public:
		//! Builds APICSolver3.
		APICSolver3 Build() const;

		//! Builds shared pointer of APICSolver3 instance.
		APICSolver3Ptr MakeShared() const;
	};
}

#endif