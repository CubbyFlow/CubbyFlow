/*************************************************************************
> File Name: FDMUtils.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Finite Difference Method util functions for CubbyFlow.
> Created Time: 2017/07/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/FDM/FDMUtils.h>

namespace CubbyFlow
{
	Vector2D Gradient2(const ConstArrayAccessor2<double>& data,
		const Vector2D& gridSpacing, size_t i, size_t j)
	{
		const Size2 ds = data.size();

		assert(i < ds.x && j < ds.y);

		double left = data((i > 0) ? i - 1 : i, j);
		double right = data((i + 1 < ds.x) ? i + 1 : i, j);
		double down = data(i, (j > 0) ? j - 1 : j);
		double up = data(i, (j + 1 < ds.y) ? j + 1 : j);

		return 0.5 * Vector2D(right - left, up - down) / gridSpacing;
	}

	std::array<Vector2D, 2> Gradient2(const ConstArrayAccessor2<Vector2D>& data,
		const Vector2D& gridSpacing, size_t i, size_t j)
	{
		const Size2 ds = data.size();

		assert(i < ds.x && j < ds.y);

		Vector2D left = data((i > 0) ? i - 1 : i, j);
		Vector2D right = data((i + 1 < ds.x) ? i + 1 : i, j);
		Vector2D down = data(i, (j > 0) ? j - 1 : j);
		Vector2D up = data(i, (j + 1 < ds.y) ? j + 1 : j);

		std::array<Vector2D, 2> result;
		result[0] = 0.5 * Vector2D(right.x - left.x, up.x - down.x) / gridSpacing;
		result[1] = 0.5 * Vector2D(right.y - left.y, up.y - down.y) / gridSpacing;

		return result;
	}

	Vector3D Gradient3(const ConstArrayAccessor3<double>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
	{
		const Size3 ds = data.size();

		assert(i < ds.x && j < ds.y && k < ds.z);

		double left = data((i > 0) ? i - 1 : i, j, k);
		double right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
		double down = data(i, (j > 0) ? j - 1 : j, k);
		double up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
		double back = data(i, j, (k > 0) ? k - 1 : k);
		double front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

		return 0.5 * Vector3D(right - left, up - down, front - back) / gridSpacing;
	}

	std::array<Vector3D, 3> Gradient3(const ConstArrayAccessor3<Vector3D>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
	{
		const Size3 ds = data.size();

		assert(i < ds.x && j < ds.y && k < ds.z);

		Vector3D left = data((i > 0) ? i - 1 : i, j, k);
		Vector3D right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
		Vector3D down = data(i, (j > 0) ? j - 1 : j, k);
		Vector3D up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
		Vector3D back = data(i, j, (k > 0) ? k - 1 : k);
		Vector3D front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

		std::array<Vector3D, 3> result;
		result[0] = 0.5 * Vector3D(right.x - left.x, up.x - down.x, front.x - back.x) / gridSpacing;
		result[1] = 0.5 * Vector3D(right.y - left.y, up.y - down.y, front.y - back.y) / gridSpacing;
		result[2] = 0.5 * Vector3D(right.z - left.z, up.z - down.z, front.z - back.z) / gridSpacing;

		return result;
	}

	double Laplacian2(const ConstArrayAccessor2<double>& data,
		const Vector2D& gridSpacing, size_t i, size_t j)
	{
		const double center = data(i, j);
		const Size2 ds = data.size();

		assert(i < ds.x && j < ds.y);

		double dLeft = 0.0, dRight = 0.0, dDown = 0.0, dUp = 0.0;

		if (i > 0)
		{
			dLeft = center - data(i - 1, j);
		}
		if (i + 1 < ds.x)
		{
			dRight = data(i + 1, j) - center;
		}

		if (j > 0)
		{
			dDown = center - data(i, j - 1);
		}
		if (j + 1 < ds.y)
		{
			dUp = data(i, j + 1) - center;
		}

		return (dRight - dLeft) / Square(gridSpacing.x) +
			(dUp - dDown) / Square(gridSpacing.y);
	}

	Vector2D Laplacian2(const ConstArrayAccessor2<Vector2D>& data,
		const Vector2D& gridSpacing, size_t i, size_t j)
	{
		const Vector2D center = data(i, j);
		const Size2 ds = data.size();

		assert(i < ds.x && j < ds.y);

		Vector2D dLeft, dRight, dDown, dUp;

		if (i > 0)
		{
			dLeft = center - data(i - 1, j);
		}
		if (i + 1 < ds.x)
		{
			dRight = data(i + 1, j) - center;
		}

		if (j > 0) 
		{
			dDown = center - data(i, j - 1);
		}
		if (j + 1 < ds.y)
		{
			dUp = data(i, j + 1) - center;
		}

		return (dRight - dLeft) / Square(gridSpacing.x) +
			(dUp - dDown) / Square(gridSpacing.y);
	}

	double Laplacian3(const ConstArrayAccessor3<double>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
	{
		const double center = data(i, j, k);
		const Size3 ds = data.size();

		assert(i < ds.x && j < ds.y && k < ds.z);

		double dLeft = 0.0, dRight = 0.0, dDown = 0.0, dUp = 0.0, dback = 0.0, dFront = 0.0;

		if (i > 0)
		{
			dLeft = center - data(i - 1, j, k);
		}
		if (i + 1 < ds.x)
		{
			dRight = data(i + 1, j, k) - center;
		}

		if (j > 0)
		{
			dDown = center - data(i, j - 1, k);
		}
		if (j + 1 < ds.y)
		{
			dUp = data(i, j + 1, k) - center;
		}

		if (k > 0)
		{
			dback = center - data(i, j, k - 1);
		}
		if (k + 1 < ds.z)
		{
			dFront = data(i, j, k + 1) - center;
		}

		return (dRight - dLeft) / Square(gridSpacing.x) +
			(dUp - dDown) / Square(gridSpacing.y) +
			(dFront - dback) / Square(gridSpacing.z);
	}

	Vector3D Laplacian3(const ConstArrayAccessor3<Vector3D>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
	{
		const Vector3D center = data(i, j, k);
		const Size3 ds = data.size();

		assert(i < ds.x && j < ds.y && k < ds.z);

		Vector3D dLeft, dRight, dDown, dUp, dBack, dFront;

		if (i > 0)
		{
			dLeft = center - data(i - 1, j, k);
		}
		if (i + 1 < ds.x)
		{
			dRight = data(i + 1, j, k) - center;
		}

		if (j > 0)
		{
			dDown = center - data(i, j - 1, k);
		}
		if (j + 1 < ds.y)
		{
			dUp = data(i, j + 1, k) - center;
		}

		if (k > 0)
		{
			dBack = center - data(i, j, k - 1);
		}
		if (k + 1 < ds.z)
		{
			dFront = data(i, j, k + 1) - center;
		}

		return (dRight - dLeft) / Square(gridSpacing.x) +
			(dUp - dDown) / Square(gridSpacing.y) +
			(dFront - dBack) / Square(gridSpacing.z);
	}
}