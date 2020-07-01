/*************************************************************************
> File Name: OfflineSimulation.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: A fluid simulator with prepared offline simulated particle files.
> Created Time: 2020/06/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "OfflineSimulation.h"

#include <Framework/Renderer/Renderer.h>
#include <Framework/Utils/ParticleParser.h>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit3.h>
#include <Core/Grid/ScalarGrid3.h>
#include <Core/Grid/VertexCenteredScalarGrid3.h>
#include <Core/Geometry/TriangleMesh3.h>
#include <Core/MarchingCubes/MarchingCubes.h>

using namespace CubbyFlow;
using namespace CubbyRender;

OfflineSimulation::OfflineSimulation()
{
    //! Do nothing
}

OfflineSimulation::OfflineSimulation(double fps)
    : _fps(fps)
{
    //! Do nothing
}

OfflineSimulation::OfflineSimulation(double fps, Vector3F origin)
    : _simulationOrigin(origin), _fps(fps)
{
    //! Do nothing
}

OfflineSimulation::~OfflineSimulation()
{
    //! Do nothing
}

void OfflineSimulation::setParticleParser(ParticleParserPtr parser)
{
    _particleParser = parser;
}

void OfflineSimulation::setup(RendererPtr renderer)
{
    _frame = Frame(0, 1.0 / _fps);

    _renderable = std::make_shared<PointsRenderable>();
    renderer->addRenderable(_renderable);
}

void OfflineSimulation::resetSimulation()
{
    _frame = Frame(0, 1.0 / _fps);
}

void OfflineSimulation::updateRenderable()
{
    const ConstArrayAccessor1<Vector3D> dParticles = _particleParser->getParticles(static_cast<size_t>(_frame.index));

    //AnisotropicPointsToImplicit3 converter(_kernelRadius, _anisoCutOffDensity, _anisoPositionSmoothingFactor, _anisoMinNumNeighbors, false);
    //Size3 resolution(100, 100, 100);
    //Vector3D gridSpacing(0.01, 0.01, 0.01);
    //VertexCenteredScalarGrid3 sdf(resolution, gridSpacing);
    //converter.Convert(dParticles, &sdf);
    //TriangleMesh3 mesh;
    //MarchingCubes(
    //    sdf.GetConstDataAccessor(),
    //    sdf.GridSpacing(),
    //    sdf.GetDataOrigin(),
    //    &mesh,
    //    0.0,
    //    DIRECTION_ALL);
    //UNUSED_VARIABLE(mesh);

    Array1<Vector3F> fParticles(dParticles.size());
    fParticles.ParallelForEachIndex([&](size_t i){
        fParticles[i] = dParticles[i].CastTo<float>() + _simulationOrigin;
    });
    _renderable->update(fParticles);

    advanceSimulation();
}

void OfflineSimulation::advanceSimulation()
{
    _frame.Advance();
    if (_frame.index == static_cast<int>(_particleParser->getNumberOfParticleCache()))
    {
        resetSimulation();
    }
}
