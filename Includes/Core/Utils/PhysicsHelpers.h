/*************************************************************************
> File Name: PhysicsHelpers.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Physics helpers for CubbyFlow.
> Created Time: 2017/06/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PHYSICS_HELPERS_H
#define CUBBYFLOW_PHYSICS_HELPERS_H

#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>

namespace CubbyFlow
{
	inline Vector2D ComputeDragForce(double dragCoefficient, double radius, const Vector2D& velocity)
	{
		// Stoke's drag force assuming our Reynolds number is very low.
		// http://en.wikipedia.org/wiki/Drag_(physics)#Very_low_Reynolds_numbers:_Stokes.27_drag
		return -6.0 * PI_DOUBLE * dragCoefficient * radius * velocity;
	}

	inline Vector3D ComputeDragForce(double dragCoefficient, double radius, const Vector3D& velocity)
	{
		// Stoke's drag force assuming our Reynolds number is very low.
		// http://en.wikipedia.org/wiki/Drag_(physics)#Very_low_Reynolds_numbers:_Stokes.27_drag
		return -6.0 * PI_DOUBLE * dragCoefficient * radius * velocity;
	}

	template <size_t N>
	inline Vector<double, N> ProjectAndApplyFriction(const Vector<double, N>& vel, const Vector<double, N>& normal, double frictionCoefficient)
	{
		Vector<double, N> velt = vel.Projected(normal);
		if (velt.LengthSquared() > 0)
		{
			double veln = std::max(-vel.Dot(normal), 0.0);
			velt *= std::max(1.0 - frictionCoefficient * veln / velt.Length(), 0.0);
		}

		return velt;
	}

	inline double ComputePressureFromEos(
		double density, double targetDensity,
		double eosScale, double eosExponent,
		double negativePressureScale)
	{
		// Equation of state
		// (http://www.ifi.uzh.ch/vmml/publications/pcisph/pcisph.pdf)
		double p = eosScale / eosExponent * (std::pow((density / targetDensity), eosExponent) - 1.0);

		// Negative pressure scaling
		if (p < 0)
		{
			p *= negativePressureScale;
		}

		return p;
	}
}

#endif