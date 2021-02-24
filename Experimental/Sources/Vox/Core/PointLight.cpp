/*************************************************************************
> File Name: PointLight.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: point light emitter class
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/PointLight.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Utils/VectorUtils.hpp>
#include <Vox/Scene/VoxScene.hpp>

namespace Vox
{
	void PointLight::AddLight(const CubbyFlow::Vector3F& position, const CubbyFlow::Vector3F& color)
	{
		_positions.Append(position);
		_colors.Append(color);
		++_numLights;
	}

	void PointLight::UploadToProgram(const std::shared_ptr<Program>& program)
	{
		auto& params = program->GetParameters();
		params.SetParameter("numLights", _numLights);
		for (int i = 0; i < _numLights; ++i)
		{
			params.SetParameter("light[" + std::to_string(i) +"].position", _positions[i]);
			params.SetParameter("light[" + std::to_string(i) +"].color", _colors[i]);
		}
	}

	void PointLight::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
	{
		UNUSED_VARIABLE(scene);
		for (const auto& source : node)
		{
			const CubbyFlow::Vector3F position = ParseFromString<float, 3>(source.attribute("pos").value());
			const CubbyFlow::Vector3F color = ParseFromString<float, 3>(source.attribute("color").value());
			AddLight(position, color);
		}
	}

	void PointLight::WriteXMLNode(pugi::xml_node& node)
	{
		UNUSED_VARIABLE(node);
	}

};  // namespace Vox