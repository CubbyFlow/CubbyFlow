/*************************************************************************
> File Name: SPHStdKernel3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Standard 3-D SPH kernel function object.
> Created Time: 2017/06/03
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/

// Adopted from the sample code of:
// Bart Adams and Martin Wicke,
// "Meshless Approximation Methods and Applications in Physics Based Modeling
// and Animation", Eurographics 2009 Tutorial

#include <SPH/SPHStdKernel3.h>
#include <Utils/Constants.h>

namespace CubbyFlow
{
	SPHStdKernel3::SPHStdKernel3() :
		h(0), h2(0), h3(0), h5(0)
	{
		// Do nothing
	}

	SPHStdKernel3::SPHStdKernel3(double h_) :
		h(h_), h2(h * h), h3(h2 * h), h5(h2 * h2 * h)
	{
		// Do nothing
	}

	SPHStdKernel3::SPHStdKernel3(const SPHStdKernel3& other) :
		h(other.h), h2(other.h2), h3(other.h3), h5(other.h5)
	{
		// Do nothing
	}

	double SPHStdKernel3::operator()(double distance) const
	{
		double distanceSquared = distance * distance;

		if (distanceSquared >= h2)
		{
			return 0.0;
		}

		double x = 1.0 - distanceSquared / h2;
		return 315.0 / (64.0 * PI_DOUBLE * h3) * x * x * x;
	}

	double SPHStdKernel3::FirstDerivative(double distance) const
	{
		if (distance >= h)
		{
			return 0.0;
		}

		double x = 1.0 - distance * distance / h2;
		return -945.0 * distance / (32.0 * PI_DOUBLE * h5) * x * x;
	}

	Vector3D SPHStdKernel3::Gradient(const Vector3D& point) const
	{
		double dist = point.Length();

		if (dist > 0.0)
		{
			return Gradient(dist, point / dist);
		}

		return Vector3D(0, 0, 0);
	}

	Vector3D SPHStdKernel3::Gradient(double distance, const Vector3D& directionToCenter) const
	{
		return -FirstDerivative(distance) * directionToCenter;
	}

	double SPHStdKernel3::SecondDerivative(double distance) const
	{
		double distanceSquared = distance * distance;

		if (distanceSquared >= h2)
		{
			return 0.0;
		}

		double x = distanceSquared / h2;
		return 945.0 / (32.0 * PI_DOUBLE * h5) * (1 - x) * (5 * x - 1);
	}

	SPHSpikyKernel3::SPHSpikyKernel3() :
		h(0), h2(0), h3(0), h4(0), h5(0)
	{
		// Do nothing
	}

	SPHSpikyKernel3::SPHSpikyKernel3(double h_) :
		h(h_), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h3 * h2)
	{
		// Do nothing
	}

	SPHSpikyKernel3::SPHSpikyKernel3(const SPHSpikyKernel3& other) :
		h(other.h), h2(other.h2), h3(other.h3), h4(other.h4), h5(other.h5)
	{
		// Do nothing
	}

	double SPHSpikyKernel3::operator()(double distance) const
	{
		if (distance >= h)
		{
			return 0.0;
		}

		double x = 1.0 - distance / h;
		return 15.0 / (PI_DOUBLE * h3) * x * x * x;
	}

	double SPHSpikyKernel3::FirstDerivative(double distance) const
	{
		if (distance >= h)
		{
			return 0.0;
		}

		double x = 1.0 - distance / h;
		return -45.0 / (PI_DOUBLE * h4) * x * x;
	}

	Vector3D SPHSpikyKernel3::Gradient(const Vector3D& point) const
	{
		double dist = point.Length();

		if (dist > 0.0)
		{
			return Gradient(dist, point / dist);
		}

		return Vector3D(0, 0, 0);
	}

	Vector3D SPHSpikyKernel3::Gradient(double distance, const Vector3D& directionToCenter) const
	{
		return -FirstDerivative(distance) * directionToCenter;
	}

	double SPHSpikyKernel3::SecondDerivative(double distance) const
	{
		if (distance >= h)
		{
			return 0.0;
		}

		double x = 1.0 - distance / h;
		return 90.0 / (PI_DOUBLE * h5) * x;
	}
}