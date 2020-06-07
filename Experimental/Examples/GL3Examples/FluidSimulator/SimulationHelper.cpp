/*************************************************************************
> File Name: SimulationHelper.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GUI System implemented with imgui for rendering simulations
> Created Time: 2020/06/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "SimulationHelper.h"

#include <Framework/Utils/Common.h>

#include <GL3/Utils/GL3Common.h>
#include <GL3/Texture/GL3Texture2D.h>

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <cassert>

using namespace CubbyFlow;
using namespace CubbyRender;

SimulationHelper::SimulationHelper()
{
	//! Do nothing.
}

SimulationHelper::SimulationHelper(Size2 windowSize)
	: _windowSize(windowSize)
{

}

SimulationHelper::~SimulationHelper()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool SimulationHelper::initialize(GLFWwindow* window)
{
	using namespace ImGui;

	CreateContext();
	ImGuiIO& io = GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //! Enable Keyboard Control
	//! Setup ImGui style.
	StyleColorsDark();
	//! Setup Platform/Renderer bindings.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	//! Handle fonts.
	io.Fonts->AddFontDefault();

	return true;
}

void SimulationHelper::render() const
{
	ImGui::Begin("Scene Window");
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	for (auto& texture : _simulationTextures)
	{
		GLuint id = texture->getGLTextureID();
		Size2 size = texture->getTextureSize();
		drawList->AddImage(static_cast<void*>(&id), ImVec2(0, 0), ImVec2(100, 100));
	}
	ImGui::End();
}

void SimulationHelper::beginRender() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void SimulationHelper::endRender() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SimulationHelper::addSimulationTexture(Texture2DPtr texture)
{
	GL3Texture2DPtr gl3Texture = std::dynamic_pointer_cast<GL3Texture2D>(texture);
	if (!gl3Texture)
	{
		CUBBYFLOW_ERROR << "Failed to dynamic casting to opengl texture instance";
		std::abort();
	}
	_simulationTextures.push_back(gl3Texture);
}