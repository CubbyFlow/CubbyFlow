/*************************************************************************
> File Name: CustomScalarField3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D scalar field with custom field function.
> Created Time: 2017/03/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Field/CustomScalarField3.h>

namespace CubbyFlow
{
	CustomScalarField3::CustomScalarField3(
		const std::function<double(const Vector3D&)>& customFunction,
		double derivativeResolution) :
		m_customFunction(customFunction),
		m_resolution(derivativeResolution)
	{
		// Do nothing
	}

	CustomScalarField3::CustomScalarField3(
		const std::function<double(const Vector3D&)>& customFunction,
		const std::function<Vector3D(const Vector3D&)>& customGradientFunction,
		double derivativeResolution) :
		m_customFunction(customFunction),
		m_customGradientFunction(customGradientFunction),
		m_resolution(derivativeResolution)
	{
		// Do nothing
	}

	CustomScalarField3::CustomScalarField3(
		const std::function<double(const Vector3D&)>& customFunction,
		const std::function<Vector3D(const Vector3D&)>& customGradientFunction,
		const std::function<double(const Vector3D&)>& customLaplacianFunction) :
		m_customFunction(customFunction),
		m_customGradientFunction(customGradientFunction),
		m_customLaplacianFunction(customLaplacianFunction),
		m_resolution(1e-3)
	{
		// Do nothing
	}

	double CustomScalarField3::Sample(const Vector3D& x) const
	{
		return m_customFunction(x);
	}

	std::function<double(const Vector3D&)> CustomScalarField3::Sampler() const
	{
		return m_customFunction;
	}

	Vector3D CustomScalarField3::Gradient(const Vector3D& x) const
	{
		if (m_customGradientFunction)
		{
			return m_customGradientFunction(x);
		}

		double left	= m_customFunction(x - Vector3D(0.5 * m_resolution, 0.0, 0.0));
		double right = m_customFunction(x + Vector3D(0.5 * m_resolution, 0.0, 0.0));
		double bottom = m_customFunction(x - Vector3D(0.0, 0.5 * m_resolution, 0.0));
		double top = m_customFunction(x + Vector3D(0.0, 0.5 * m_resolution, 0.0));
		double back = m_customFunction(x - Vector3D(0.0, 0.0, 0.5 * m_resolution));
		double front = m_customFunction(x + Vector3D(0.0, 0.0, 0.5 * m_resolution));

		return Vector3D((right - left) / m_resolution, (top - bottom) / m_resolution, (front - back) / m_resolution);
	}

	double CustomScalarField3::Laplacian(const Vector3D& x) const
	{
		if (m_customLaplacianFunction)
		{
			return m_customLaplacianFunction(x);
		}

		double center = m_customFunction(x);
		double left = m_customFunction(x - Vector3D(0.5 * m_resolution, 0.0, 0.0));
		double right = m_customFunction(x + Vector3D(0.5 * m_resolution, 0.0, 0.0));
		double bottom = m_customFunction(x - Vector3D(0.0, 0.5 * m_resolution, 0.0));
		double top = m_customFunction(x + Vector3D(0.0, 0.5 * m_resolution, 0.0));
		double back = m_customFunction(x - Vector3D(0.0, 0.0, 0.5 * m_resolution));
		double front = m_customFunction(x + Vector3D(0.0, 0.0, 0.5 * m_resolution));

		return (left + right + bottom + top + back + front - 6.0 * center) / (m_resolution * m_resolution);
	}

	CustomScalarField3::Builder CustomScalarField3::GetBuilder()
	{
		return Builder();
	}

	CustomScalarField3::Builder& CustomScalarField3::Builder::WithFunction(const std::function<double(const Vector3D&)>& func)
	{
		m_customFunction = func;
		return *this;
	}

	CustomScalarField3::Builder& CustomScalarField3::Builder::WithGradientFunction(const std::function<Vector3D(const Vector3D&)>& func)
	{
		m_customGradientFunction = func;
		return *this;
	}

	CustomScalarField3::Builder& CustomScalarField3::Builder::WithLaplacianFunction(const std::function<double(const Vector3D&)>& func)
	{
		m_customLaplacianFunction = func;
		return *this;
	}

	CustomScalarField3::Builder& CustomScalarField3::Builder::WithDerivativeResolution(double resolution)
	{
		m_resolution = resolution;
		return *this;
	}

	CustomScalarField3 CustomScalarField3::Builder::Build() const
	{
		if (m_customLaplacianFunction)
		{
			return CustomScalarField3(m_customFunction, m_customGradientFunction, m_customLaplacianFunction);
		}

		return CustomScalarField3(m_customFunction, m_customGradientFunction, m_resolution);
	}

	CustomScalarField3Ptr CustomScalarField3::Builder::MakeShared() const
	{
		if (m_customLaplacianFunction)
		{
			return std::shared_ptr<CustomScalarField3>(
				new CustomScalarField3(m_customFunction, m_customGradientFunction, m_customLaplacianFunction),
				[](CustomScalarField3* obj)
			{
				delete obj;
			});
		}

		return std::shared_ptr<CustomScalarField3>(
			new CustomScalarField3(m_customFunction, m_customGradientFunction, m_resolution),
			[](CustomScalarField3* obj)
		{
			delete obj;
		});
	}
}