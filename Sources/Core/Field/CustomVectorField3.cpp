/*************************************************************************
> File Name: CustomVectorField3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D vector field with custom field function.
> Created Time: 2017/03/24
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Field/CustomVectorField3.h>

namespace CubbyFlow
{
	CustomVectorField3::CustomVectorField3(
		const std::function<Vector3D(const Vector3D&)>& customFunction,
		double derivativeResolution) :
		m_customFunction(customFunction),
		m_resolution(derivativeResolution)
	{
		// Do nothing
	}

	CustomVectorField3::CustomVectorField3(
		const std::function<Vector3D(const Vector3D&)>& customFunction,
		const std::function<double(const Vector3D&)>& customDivergenceFunction,
		double derivativeResolution) :
		m_customFunction(customFunction),
		m_customDivergenceFunction(customDivergenceFunction),
		m_resolution(derivativeResolution)
	{
		// Do nothing
	}

	CustomVectorField3::CustomVectorField3(
		const std::function<Vector3D(const Vector3D&)>& customFunction,
		const std::function<double(const Vector3D&)>& customDivergenceFunction,
		const std::function<Vector3D(const Vector3D&)>& customCurlFunction) :
		m_customFunction(customFunction),
		m_customDivergenceFunction(customDivergenceFunction),
		m_customCurlFunction(customCurlFunction)
	{
		// Do nothing
	}

	Vector3D CustomVectorField3::Sample(const Vector3D& x) const
	{
		return m_customFunction(x);
	}

	std::function<Vector3D(const Vector3D&)> CustomVectorField3::Sampler() const
	{
		return m_customFunction;
	}

	double CustomVectorField3::Divergence(const Vector3D& x) const
	{
		if (m_customDivergenceFunction)
		{
			return m_customDivergenceFunction(x);
		}

		double left	= m_customFunction(x - Vector3D(0.5 * m_resolution, 0.0, 0.0)).x;
		double right = m_customFunction(x + Vector3D(0.5 * m_resolution, 0.0, 0.0)).x;
		double bottom = m_customFunction(x - Vector3D(0.0, 0.5 * m_resolution, 0.0)).y;
		double top = m_customFunction(x + Vector3D(0.0, 0.5 * m_resolution, 0.0)).y;
		double back = m_customFunction(x - Vector3D(0.0, 0.0, 0.5 * m_resolution)).z;
		double front = m_customFunction(x + Vector3D(0.0, 0.0, 0.5 * m_resolution)).z;

		return (right - left + top - bottom + front - back) / m_resolution;
	}

	Vector3D CustomVectorField3::Curl(const Vector3D& x) const
	{
		if (m_customCurlFunction)
		{
			return m_customCurlFunction(x);
		}

		Vector3D left = m_customFunction(x - Vector3D(0.5 * m_resolution, 0.0, 0.0));
		Vector3D right = m_customFunction(x + Vector3D(0.5 * m_resolution, 0.0, 0.0));
		Vector3D bottom = m_customFunction(x - Vector3D(0.0, 0.5 * m_resolution, 0.0));
		Vector3D top = m_customFunction(x + Vector3D(0.0, 0.5 * m_resolution, 0.0));
		Vector3D back = m_customFunction(x - Vector3D(0.0, 0.0, 0.5 * m_resolution));
		Vector3D front = m_customFunction(x + Vector3D(0.0, 0.0, 0.5 * m_resolution));

		double Fx_ym = bottom.x;
		double Fx_yp = top.x;
		double Fx_zm = back.x;
		double Fx_zp = front.x;

		double Fy_xm = left.y;
		double Fy_xp = right.y;
		double Fy_zm = back.y;
		double Fy_zp = front.y;

		double Fz_xm = left.z;
		double Fz_xp = right.z;
		double Fz_ym = bottom.z;
		double Fz_yp = top.z;

		return Vector3D((Fz_yp - Fz_ym) - (Fy_zp - Fy_zm), (Fx_zp - Fx_zm) - (Fz_xp - Fz_xm), (Fy_xp - Fy_xm) - (Fx_yp - Fx_ym)) / m_resolution;
	}

	CustomVectorField3::Builder CustomVectorField3::GetBuilder()
	{
		return Builder();
	}

	CustomVectorField3::Builder& CustomVectorField3::Builder::WithFunction(const std::function<Vector3D(const Vector3D&)>& func)
	{
		m_customFunction = func;
		return *this;
	}

	CustomVectorField3::Builder& CustomVectorField3::Builder::WithDivergenceFunction(const std::function<double(const Vector3D&)>& func)
	{
		m_customDivergenceFunction = func;
		return *this;
	}

	CustomVectorField3::Builder& CustomVectorField3::Builder::WithCurlFunction(const std::function<Vector3D(const Vector3D&)>& func)
	{
		m_customCurlFunction = func;
		return *this;
	}

	CustomVectorField3::Builder& CustomVectorField3::Builder::WithDerivativeResolution(double resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	CustomVectorField3 CustomVectorField3::Builder::Build() const
	{
		if (m_customCurlFunction)
		{
			return CustomVectorField3(m_customFunction, m_customDivergenceFunction, m_customCurlFunction);
		}

		return CustomVectorField3(m_customFunction, m_customDivergenceFunction, m_resolution);
	}

	CustomVectorField3Ptr CustomVectorField3::Builder::MakeShared() const
	{
		if (m_customCurlFunction)
		{
			return std::shared_ptr<CustomVectorField3>(
				new CustomVectorField3(m_customFunction, m_customDivergenceFunction, m_customCurlFunction),
				[](CustomVectorField3* obj)
			{
				delete obj;
			});
		}

		return std::shared_ptr<CustomVectorField3>(
			new CustomVectorField3(m_customFunction, m_customDivergenceFunction, m_resolution),
			[](CustomVectorField3* obj)
		{
			delete obj;
		});
	}
}