/*************************************************************************
> File Name: PointLight.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base emitter class
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/PointLight.hpp>
#include <Vox/Core/Program.hpp>

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

};  // namespace Vox