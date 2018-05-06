/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Main entry of CubbyFlow Python API.
> Created Time: 2018/01/22
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Array/ArrayAccessor.h>
#include <API/Python/Animation/Animation.h>
#include <API/Python/Animation/Frame.h>
#include <API/Python/Animation/PhysicsAnimation.h>
#include <API/Python/BoundingBox/BoundingBox.h>
#include <API/Python/Collider/Collider.h>
#include <API/Python/Collider/RigidbodyCollider.h>
#include <API/Python/Emitter/GridEmitter.h>
#include <API/Python/Emitter/VolumeGridEmitter.h>
#include <API/Python/Emitter/ParticleEmitter.h>
#include <API/Python/Emitter/ParticleEmitterSet.h>
#include <API/Python/Emitter/PointParticleEmitter.h>
#include <API/Python/Emitter/VolumeParticleEmitter.h>
#include <API/Python/Field/Field.h>
#include <API/Python/Field/ScalarField.h>
#include <API/Python/Field/VectorField.h>
#include <API/Python/Field/ConstantScalarField.h>
#include <API/Python/Field/ConstantVectorField.h>
#include <API/Python/Field/CustomScalarField.h>
#include <API/Python/Field/CustomVectorField.h>
#include <API/Python/Geometry/Box.h>
#include <API/Python/Geometry/Cylinder.h>
#include <API/Python/Geometry/Plane.h>
#include <API/Python/Geometry/Sphere.h>
#include <API/Python/Geometry/Triangle.h>
#include <API/Python/Geometry/TriangleMesh.h>
#include <API/Python/Grid/Grid.h>
#include <API/Python/Grid/GridSystemData.h>
#include <API/Python/Grid/ScalarGrid.h>
#include <API/Python/Grid/VectorGrid.h>
#include <API/Python/Grid/CollocatedVectorGrid.h>
#include <API/Python/Grid/CellCenteredScalarGrid.h>
#include <API/Python/Grid/CellCenteredVectorGrid.h>
#include <API/Python/Grid/VertexCenteredScalarGrid.h>
#include <API/Python/Grid/VertexCenteredVectorGrid.h>
#include <API/Python/Grid/FaceCenteredGrid.h>
#include <API/Python/Math/Quaternion.h>
#include <API/Python/Particle/ParticleSystemData.h>
#include <API/Python/Point/Point.h>
#include <API/Python/PointsToImplicit/PointsToImplicit.h>
#include <API/Python/PointsToImplicit/AnisotropicPointsToImplicit.h>
#include <API/Python/PointsToImplicit/SphericalPointsToImplicit.h>
#include <API/Python/PointsToImplicit/SPHPointsToImplicit.h>
#include <API/Python/PointsToImplicit/ZhuBridsonPointsToImplicit.h>
#include <API/Python/Ray/Ray.h>
#include <API/Python/SemiLagrangian/SemiLagrangian.h>
#include <API/Python/SemiLagrangian/CubicSemiLagrangian.h>
#include <API/Python/Size/Size.h>
#include <API/Python/Solver/Advection/AdvectionSolver.h>
#include <API/Python/Solver/FDM/FDMLinearSystemSolver.h>
#include <API/Python/Solver/FDM/FDMJacobiSolver.h>
#include <API/Python/Solver/FDM/FDMGaussSeidelSolver.h>
#include <API/Python/Solver/FDM/FDMCGSolver.h>
#include <API/Python/Solver/FDM/FDMICCGSolver.h>
#include <API/Python/Solver/FDM/FDMMGSolver.h>
#include <API/Python/Solver/FDM/FDMMGPCGSolver.h>
#include <API/Python/Solver/Grid/GridDiffusionSolver.h>
#include <API/Python/Solver/Grid/GridForwardEulerDiffusionSolver.h>
#include <API/Python/Solver/Grid/GridBackwardEulerDiffusionSolver.h>
#include <API/Python/Solver/Grid/GridBoundaryConditionSolver.h>
#include <API/Python/Solver/Grid/GridFractionalBoundaryConditionSolver.h>
#include <API/Python/Solver/Grid/GridBlockedBoundaryConditionSolver.h>
#include <API/Python/Solver/Grid/GridPressureSolver.h>
#include <API/Python/Solver/Grid/GridSinglePhasePressureSolver.h>
#include <API/Python/Solver/Grid/GridFractionalSinglePhasePressureSolver.h>
#include <API/Python/Solver/LevelSet/LevelSetSolver.h>
#include <API/Python/Solver/LevelSet/IterativeLevelSetSolver.h>
#include <API/Python/Solver/LevelSet/UpwindLevelSetSolver.h>
#include <API/Python/Solver/LevelSet/ENOLevelSetSolver.h>
#include <API/Python/Solver/LevelSet/FMMLevelSetSolver.h>
#include <API/Python/Solver/Grid/GridFluidSolver.h>
#include <API/Python/Solver/Grid/GridSmokeSolver.h>
#include <API/Python/Solver/LevelSet/LevelSetLiquidSolver.h>
#include <API/Python/Solver/Hybrid/PIC/PICSolver.h>
#include <API/Python/Solver/Hybrid/FLIP/FLIPSolver.h>
#include <API/Python/Solver/Hybrid/APIC/APICSolver.h>
#include <API/Python/Solver/Particle/ParticleSystemSolver.h>
#include <API/Python/Solver/Particle/SPH/SPHSolver.h>
#include <API/Python/Solver/Particle/PCISPH/PCISPHSolver.h>
#include <API/Python/SPH/SPHSystemData.h>
#include <API/Python/Surface/Surface.h>
#include <API/Python/Surface/SurfaceSet.h>
#include <API/Python/Surface/ImplicitSurface.h>
#include <API/Python/Surface/SurfaceToImplicit.h>
#include <API/Python/Transform/Transform.h>
#include <API/Python/Utils/Constants.h>
#include <API/Python/Utils/Logging.h>
#include <API/Python/Utils/Serializable.h>
#include <API/Python/Vector/Vector.h>

#include <pybind11/pybind11.h>

PYBIND11_MODULE(pyCubbyFlow, m)
{
	m.doc() =
		R"pbdoc(
			Voxel-based fluid simulation engine for computer games
		)pbdoc";

	// Constants
	AddConstants(m);

	// Serialization
	AddSerializable(m);

	// Trivial basic types
	AddVector2D(m);
	AddVector2F(m);
	AddVector3D(m);
	AddVector3F(m);

	// Rays
	AddRay2D(m);
	AddRay2F(m);
	AddRay3D(m);
	AddRay3F(m);

	// Bounding boxes
	AddBoundingBox2D(m);
	AddBoundingBox2F(m);
	AddBoundingBox3D(m);
	AddBoundingBox3F(m);

	// Frames
	AddFrame(m);

	// Quaternions
	AddQuaternionD(m);
	AddQuaternionF(m);

	// Points
	AddPoint2UI(m);
	AddPoint3UI(m);

	// Sizes
	AddSize2(m);
	AddSize3(m);

	// Transforms
	AddTransform2(m);
	AddTransform3(m);

	// Containers/helpers
	AddArrayAccessor1(m);
	AddArrayAccessor2(m);
	AddArrayAccessor3(m);

	// Trivial APIs
	AddLogging(m);

	// Fields
	AddField2(m);
	AddField3(m);
	AddScalarField2(m);
	AddScalarField3(m);
	AddVectorField2(m);
	AddVectorField3(m);
	AddConstantScalarField2(m);
	AddConstantScalarField3(m);
	AddConstantVectorField2(m);
	AddConstantVectorField3(m);
	AddCustomScalarField2(m);
	AddCustomScalarField3(m);
	AddCustomVectorField2(m);
	AddCustomVectorField3(m);

	// Grids
	AddGrid2(m);
	AddGrid3(m);
	AddScalarGrid2(m);
	AddScalarGrid3(m);
	AddVectorGrid2(m);
	AddVectorGrid3(m);
	AddCollocatedVectorGrid2(m);
	AddCollocatedVectorGrid3(m);
	AddCellCenteredScalarGrid2(m);
	AddCellCenteredScalarGrid3(m);
	AddCellCenteredVectorGrid2(m);
	AddCellCenteredVectorGrid3(m);
	AddVertexCenteredScalarGrid2(m);
	AddVertexCenteredScalarGrid3(m);
	AddVertexCenteredVectorGrid2(m);
	AddVertexCenteredVectorGrid3(m);
	AddFaceCenteredGrid2(m);
	AddFaceCenteredGrid3(m);

	// Surfaces
	AddSurface2(m);
	AddSurface3(m);
	AddSurfaceSet2(m);
	AddSurfaceSet3(m);

	// Geometries
	AddBox2(m);
	AddBox3(m);
	AddCylinder3(m);
	AddPlane2(m);
	AddPlane3(m);
	AddSphere2(m);
	AddSphere3(m);
	AddTriangle3(m);
	AddTriangleMesh3(m);

	// Implicit surfaces
	AddImplicitSurface2(m);
	AddImplicitSurface3(m);
	AddSurfaceToImplicit2(m);
	AddSurfaceToImplicit3(m);

	// Data models
	AddGridSystemData2(m);
	AddGridSystemData3(m);
	AddParticleSystemData2(m);
	AddParticleSystemData3(m);
	AddSPHSystemData2(m);
	AddSPHSystemData3(m);

	// Emitters
	AddGridEmitter2(m);
	AddGridEmitter3(m);
	AddVolumeGridEmitter2(m);
	AddVolumeGridEmitter3(m);
	AddParticleEmitter2(m);
	AddParticleEmitter3(m);
	AddParticleEmitterSet2(m);
	AddParticleEmitterSet3(m);
	AddPointParticleEmitter2(m);
	AddPointParticleEmitter3(m);
	AddVolumeParticleEmitter2(m);
	AddVolumeParticleEmitter3(m);

	// Colliders
	AddCollider2(m);
	AddCollider3(m);
	AddRigidBodyCollider2(m);
	AddRigidBodyCollider3(m);

	// Advection solvers
	AddAdvectionSolver2(m);
	AddAdvectionSolver3(m);

	// Semi-lagrangians
	AddSemiLagrangian2(m);
	AddSemiLagrangian3(m);
	AddCubicSemiLagrangian2(m);
	AddCubicSemiLagrangian3(m);

	// Solvers, part 1
	AddFDMLinearSystemSolver2(m);
	AddFDMLinearSystemSolver3(m);
	AddFDMJacobiSolver2(m);
	AddFDMJacobiSolver3(m);
	AddFDMGaussSeidelSolver2(m);
	AddFDMGaussSeidelSolver3(m);
	AddFDMCGSolver2(m);
	AddFDMCGSolver3(m);
	AddFDMICCGSolver2(m);
	AddFDMICCGSolver3(m);
	AddFDMMGSolver2(m);
	AddFDMMGSolver3(m);
	AddFDMMGPCGSolver2(m);
	AddFDMMGPCGSolver3(m);
	AddGridDiffusionSolver2(m);
	AddGridDiffusionSolver3(m);
	AddGridForwardEulerDiffusionSolver2(m);
	AddGridForwardEulerDiffusionSolver3(m);
	AddGridBackwardEulerDiffusionSolver2(m);
	AddGridBackwardEulerDiffusionSolver3(m);
	AddGridBoundaryConditionSolver2(m);
	AddGridBoundaryConditionSolver3(m);
	AddGridFractionalBoundaryConditionSolver2(m);
	AddGridFractionalBoundaryConditionSolver3(m);
	AddGridBlockedBoundaryConditionSolver2(m);
	AddGridBlockedBoundaryConditionSolver3(m);
	AddGridPressureSolver2(m);
	AddGridPressureSolver3(m);
	AddGridSinglePhasePressureSolver2(m);
	AddGridSinglePhasePressureSolver3(m);
	AddGridFractionalSinglePhasePressureSolver2(m);
	AddGridFractionalSinglePhasePressureSolver3(m);
	AddLevelSetSolver2(m);
	AddLevelSetSolver3(m);
	AddIterativeLevelSetSolver2(m);
	AddIterativeLevelSetSolver3(m);
	AddUpwindLevelSetSolver2(m);
	AddUpwindLevelSetSolver3(m);
	AddENOLevelSetSolver2(m);
	AddENOLevelSetSolver3(m);
	AddFMMLevelSetSolver2(m);
	AddFMMLevelSetSolver3(m);

	// Points to implicit functions
	AddPointsToImplicit2(m);
	AddPointsToImplicit3(m);
	AddAnisotropicPointsToImplicit2(m);
	AddAnisotropicPointsToImplicit3(m);
	AddSphericalPointsToImplicit2(m);
	AddSphericalPointsToImplicit3(m);
	AddSPHPointsToImplicit2(m);
	AddSPHPointsToImplicit3(m);
	AddZhuBridsonPointsToImplicit2(m);
	AddZhuBridsonPointsToImplicit3(m);

	// Animations
	AddAnimation(m);
	AddPhysicsAnimation(m);

	// Solvers, part 2
	AddGridFluidSolver2(m);
	AddGridFluidSolver3(m);
	AddGridSmokeSolver2(m);
	AddGridSmokeSolver3(m);
	AddLevelSetLiquidSolver2(m);
	AddLevelSetLiquidSolver3(m);
	AddPICSolver2(m);
	AddPICSolver3(m);
	AddFLIPSolver2(m);
	AddFLIPSolver3(m);
	AddAPICSolver2(m);
	AddAPICSolver3(m);
	AddParticleSystemSolver2(m);
	AddParticleSystemSolver3(m);
	AddSPHSolver2(m);
	AddSPHSolver3(m);
	AddPCISPHSolver2(m);
	AddPCISPHSolver3(m);

#ifdef VERSION_INFO
	m.attr("__version__") = pybind11::str(VERSION_INFO);
#else
	m.attr("__version__") = pybind11::str("Dev");
#endif
}