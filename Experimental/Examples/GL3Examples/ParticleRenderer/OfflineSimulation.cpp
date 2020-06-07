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
    : _fps(fps), _simulationOrigin(origin)
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
    if (_frame.index % static_cast<size_t>(_fps) == 0)
    {
        auto particles = _particleParser->getParticles(static_cast<size_t>(_frame.TimeInSeconds()));
        const size_t numParticles = particles.size();

        Array1<Vector4F> colors(numParticles, Vector4F(221 / 255.0f, 145 / 255.0f, 48 / 255.0f, 1.0f));
        particles.ParallelForEachIndex([&](size_t index){
            particles[index] += _simulationOrigin;
        });

        _renderable->update(particles, colors.ConstAccessor());
    }

    advanceSimulation();
}

void OfflineSimulation::advanceSimulation()
{
    _frame.Advance();
    if (_frame.index == static_cast<size_t>(_particleParser->getNumberOfParticleState() * _fps))
    {
        resetSimulation();
    }
}
