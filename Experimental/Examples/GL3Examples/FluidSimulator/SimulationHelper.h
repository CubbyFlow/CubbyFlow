/*************************************************************************
> File Name: SimulationHelper.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GUI System implemented with imgui for rendering simulations
> Created Time: 2020/05/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef SIMULATION_HELPER_H
#define SIMULATION_HELPER_H

#include <Framework/Utils/Prerequisites.h>
#include <GL3/Utils/GL3Prerequisites.h>
#include <Core/Size/Size2.h>
#include <vector>

struct GLFWwindow;
class SimulationHelper final
{
public:
    //! Default Constructor.
    SimulationHelper();

    //! Constructor with window size;
    SimulationHelper(CubbyFlow::Size2 windowSize);

    //! Default Destructor.
    ~SimulationHelper();

    //! Initialize simulation helper with glfw window.
    bool initialize(GLFWwindow* window);

    //! render gui
    void render() const;
    void beginRender() const;
    void endRender() const;

    //! add simulation texture pointer.
    void addSimulationTexture(CubbyFlow::CubbyRender::Texture2DPtr texture);
protected:
private:

    std::vector<CubbyFlow::CubbyRender::GL3Texture2DPtr> _simulationTextures;
    CubbyFlow::Size2 _windowSize;
};

#endif