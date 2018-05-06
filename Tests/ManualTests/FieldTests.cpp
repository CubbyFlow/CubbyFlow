#include "pch.h"

#include <ManualTests.h>

#include <Core/Array/Array2.h>
#include <Core/Array/ArrayUtils.h>
#include <Core/Field/ScalarField2.h>
#include <Core/Field/ScalarField3.h>
#include <Core/Field/VectorField3.h>

using namespace CubbyFlow;

class MyCustomScalarField3 final : public ScalarField3
{
public:
	double Sample(const Vector3D& x) const override
	{
		return std::sin(x.x) * std::sin(x.y) * std::sin(x.z);
	}

	Vector3D Gradient(const Vector3D& x) const override
	{
		return Vector3D(
			std::cos(x.x) * std::sin(x.y) * std::sin(x.z),
			std::sin(x.x) * std::cos(x.y) * std::sin(x.z),
			std::sin(x.x) * std::sin(x.y) * std::cos(x.z));
	}

	double Laplacian(const Vector3D& x) const override
	{
		return
			-std::sin(x.x) * std::sin(x.y) * std::sin(x.z)
			- std::sin(x.x) * std::sin(x.y) * std::sin(x.z)
			- std::sin(x.x) * std::sin(x.y) * std::sin(x.z);
	}
};

CUBBYFLOW_TESTS(ScalarField3);

CUBBYFLOW_BEGIN_TEST_F(ScalarField3, Sample)
{
	MyCustomScalarField3 field;
	Array2<double> data(50, 50);

	for (int j = 0; j < 50; ++j)
	{
		for (int i = 0; i < 50; ++i)
		{
			Vector3D x(0.04 * PI_DOUBLE * i, 0.04 * PI_DOUBLE * j, HALF_PI_DOUBLE);
			data(i, j) = field.Sample(x);
		}
	}

	SaveData(data.ConstAccessor(), "data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ScalarField3, Gradient)
{
	MyCustomScalarField3 field;
	Array2<double> dataU(20, 20);
	Array2<double> dataV(20, 20);

	for (int j = 0; j < 20; ++j)
	{
		for (int i = 0; i < 20; ++i)
		{
			Vector3D x(0.1 * PI_DOUBLE * i, 0.1 * PI_DOUBLE * j, HALF_PI_DOUBLE);
			Vector3D g = field.Gradient(x);
			dataU(i, j) = g.x;
			dataV(i, j) = g.y;
		}
	}

	SaveData(dataU.ConstAccessor(), "data_#grid2,x.npy");
	SaveData(dataV.ConstAccessor(), "data_#grid2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ScalarField3, Laplacian)
{
	MyCustomScalarField3 field;
	Array2<double> data(50, 50);

	for (int j = 0; j < 50; ++j)
	{
		for (int i = 0; i < 50; ++i)
		{
			Vector3D x(0.04 * PI_DOUBLE * i, 0.04 * PI_DOUBLE * j, HALF_PI_DOUBLE);
			data(i, j) = field.Laplacian(x);
		}
	}

	SaveData(data.ConstAccessor(), "data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

class MyCustomVectorField3 final : public VectorField3
{
public:
	Vector3D Sample(const Vector3D& x) const override
	{
		return Vector3D(
			std::sin(x.x) * std::sin(x.y),
			std::sin(x.y) * std::sin(x.z),
			std::sin(x.z) * std::sin(x.x));
	}

	double Divergence(const Vector3D& x) const override
	{
		return std::cos(x.x) * std::sin(x.y)
			+ std::cos(x.y) * std::sin(x.z)
			+ std::cos(x.z) * std::sin(x.x);
	}

	Vector3D Curl(const Vector3D& x) const override
	{
		return Vector3D(
			-std::sin(x.y) * std::cos(x.z),
			-std::sin(x.z) * std::cos(x.x),
			-std::sin(x.x) * std::cos(x.y));
	}
};

class MyCustomVectorField3_2 final : public VectorField3
{
public:
	Vector3D Sample(const Vector3D& x) const override
	{
		return Vector3D(-x.y, x.x, 0.0);
	}
};

CUBBYFLOW_TESTS(VectorField3);

CUBBYFLOW_BEGIN_TEST_F(VectorField3, Sample)
{
	MyCustomVectorField3 field;
	Array2<double> dataU(20, 20);
	Array2<double> dataV(20, 20);

	for (int j = 0; j < 20; ++j)
	{
		for (int i = 0; i < 20; ++i)
		{
			Vector3D x(0.1 * PI_DOUBLE * i, 0.1 * PI_DOUBLE * j, HALF_PI_DOUBLE);
			dataU(i, j) = field.Sample(x).x;
			dataV(i, j) = field.Sample(x).y;
		}
	}

	SaveData(dataU.ConstAccessor(), "data_#grid2,x.npy");
	SaveData(dataV.ConstAccessor(), "data_#grid2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(VectorField3, Divergence)
{
	MyCustomVectorField3 field;
	Array2<double> data(50, 50);

	for (int j = 0; j < 50; ++j)
	{
		for (int i = 0; i < 50; ++i)
		{
			Vector3D x(0.04 * PI_DOUBLE * i, 0.04 * PI_DOUBLE * j, HALF_PI_DOUBLE);
			data(i, j) = field.Divergence(x);
		}
	}

	SaveData(data.ConstAccessor(), "data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(VectorField3, Curl)
{
	MyCustomVectorField3 field;
	Array2<double> dataU(20, 20);
	Array2<double> dataV(20, 20);

	for (int j = 0; j < 20; ++j)
	{
		for (int i = 0; i < 20; ++i)
		{
			Vector3D x(0.1 * PI_DOUBLE * i, 0.1 * PI_DOUBLE * j, 0.5 * HALF_PI_DOUBLE);
			dataU(i, j) = field.Curl(x).x;
			dataV(i, j) = field.Curl(x).y;
		}
	}

	SaveData(dataU.ConstAccessor(), "data_#grid2,x.npy");
	SaveData(dataV.ConstAccessor(), "data_#grid2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(VectorField3, Sample2)
{
	MyCustomVectorField3_2 field;
	Array2<double> dataU(20, 20);
	Array2<double> dataV(20, 20);

	for (int j = 0; j < 20; ++j)
	{
		for (int i = 0; i < 20; ++i)
		{
			Vector3D x(0.05 * i - 0.5, 0.05 * j - 0.5, 0.5);
			dataU(i, j) = field.Sample(x).x;
			dataV(i, j) = field.Sample(x).y;
		}
	}

	SaveData(dataU.ConstAccessor(), "data_#grid2,x.npy");
	SaveData(dataV.ConstAccessor(), "data_#grid2,y.npy");
}
CUBBYFLOW_END_TEST_F