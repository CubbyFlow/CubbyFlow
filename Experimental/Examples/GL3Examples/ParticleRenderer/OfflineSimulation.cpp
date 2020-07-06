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
#include <Framework/Utils/GeometryCacheParser.h>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit3.h>
#include <Core/Grid/ScalarGrid3.h>
#include <Core/Grid/VertexCenteredScalarGrid3.h>
#include <Core/Geometry/TriangleMesh3.h>
#include <Core/MarchingCubes/MarchingCubes.h>
#include <Core/Array/Array1.h>

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

void OfflineSimulation::setParticleParser(GeometryCacheParserPtr parser)
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
    _renderable->update(_particleParser->getVertexCache(static_cast<size_t>(_frame.index)));

    advanceSimulation();
}

void OfflineSimulation::advanceSimulation()
{
    _frame.Advance();
    if (_frame.index == static_cast<int>(_particleParser->getNumberOfGeometryCache()))
    {
        resetSimulation();
    }
}
