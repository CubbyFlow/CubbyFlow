#include "pch.h"

#include <Core/SPH/SPHSystemData2.hpp>

using namespace CubbyFlow;

TEST(SPHSystemData2, Parameters)
{
	SPHSystemData2 data;

	data.SetTargetDensity(123.0);
	data.SetTargetSpacing(0.549);
	data.SetRelativeKernelRadius(2.5);

	EXPECT_EQ(123.0, data.GetTargetDensity());
	EXPECT_EQ(0.549, data.GetTargetSpacing());
	EXPECT_EQ(0.549, data.GetRadius());
	EXPECT_EQ(2.5, data.GetRelativeKernelRadius());
	EXPECT_DOUBLE_EQ(2.5 * 0.549, data.GetKernelRadius());

	data.SetKernelRadius(1.9);
	EXPECT_DOUBLE_EQ(1.9, data.GetKernelRadius());
	EXPECT_DOUBLE_EQ(1.9 / 2.5, data.GetTargetSpacing());

	data.SetRadius(0.413);
	EXPECT_EQ(0.413, data.GetTargetSpacing());
	EXPECT_EQ(0.413, data.GetRadius());
	EXPECT_EQ(2.5, data.GetRelativeKernelRadius());
	EXPECT_DOUBLE_EQ(2.5 * 0.413, data.GetKernelRadius());

	data.SetMass(2.0 * data.GetMass());
	EXPECT_DOUBLE_EQ(246.0, data.GetTargetDensity());
}

TEST(SPHSystemData2, Particles)
{
	SPHSystemData2 data;

	data.SetTargetSpacing(1.0);
	data.SetRelativeKernelRadius(1.0);

	data.AddParticle(Vector2D(0, 0));
	data.AddParticle(Vector2D(1, 0));

	data.BuildNeighborSearcher();
	data.UpdateDensities();

	// See if we get symmetric density profile
	auto den = data.GetDensities();
	EXPECT_LT(0.0, den[0]);
	EXPECT_EQ(den[0], den[1]);

	Array1<double> values = { 1.0, 1.0 };
	double midVal = data.Interpolate(Vector2D(0.5, 0), values.ConstAccessor());
	EXPECT_LT(0.0, midVal);
	EXPECT_GT(1.0, midVal);
}

TEST(SPHSystemData2, Serialization)
{
	SPHSystemData2 data;
	ParticleSystemData2::VectorData positions =
	{
		{ 0.3, 0.5 },
		{ 0.6, 0.8 },
		{ 0.1, 0.8 },
		{ 0.7, 0.9 },
		{ 0.3, 0.2 },
		{ 0.8, 0.3 },
		{ 0.8, 0.5 },
		{ 0.4, 0.9 },
		{ 0.8, 0.6 },
		{ 0.2, 0.9 },
		{ 0.1, 0.2 },
		{ 0.6, 0.9 },
		{ 0.2, 0.2 },
		{ 0.5, 0.6 },
		{ 0.8, 0.4 },
		{ 0.4, 0.2 },
		{ 0.2, 0.3 },
		{ 0.8, 0.6 },
		{ 0.2, 0.8 },
		{ 1.0, 0.5 }
	};
	data.AddParticles(positions);

	data.SetTargetDensity(123.0);
	data.SetTargetSpacing(0.549);
	data.SetRelativeKernelRadius(2.5);

	size_t a0 = data.AddScalarData(2.0);
	size_t a1 = data.AddScalarData(9.0);
	size_t a2 = data.AddVectorData({ 1.0, -3.0 });

	std::vector<uint8_t> buffer;
	data.Serialize(&buffer);

	SPHSystemData2 data2;
	data2.Deserialize(buffer);

	EXPECT_EQ(123.0, data2.GetTargetDensity());
	EXPECT_EQ(0.549, data2.GetTargetSpacing());
	EXPECT_EQ(0.549, data2.GetRadius());
	EXPECT_EQ(2.5, data2.GetRelativeKernelRadius());
	EXPECT_DOUBLE_EQ(2.5 * 0.549, data2.GetKernelRadius());

	EXPECT_EQ(positions.size(), data2.GetNumberOfParticles());
	auto as0 = data2.ScalarDataAt(a0);
	for (size_t i = 0; i < positions.size(); ++i)
	{
		EXPECT_DOUBLE_EQ(2.0, as0[i]);
	}

	auto as1 = data2.ScalarDataAt(a1);
	for (size_t i = 0; i < positions.size(); ++i)
	{
		EXPECT_DOUBLE_EQ(9.0, as1[i]);
	}

	auto as2 = data2.VectorDataAt(a2);
	for (size_t i = 0; i < positions.size(); ++i)
	{
		EXPECT_DOUBLE_EQ(1.0, as2[i].x);
		EXPECT_DOUBLE_EQ(-3.0, as2[i].y);
	}

	const auto& neighborLists = data.GetNeighborLists();
	const auto& neighborLists2 = data2.GetNeighborLists();
	EXPECT_EQ(neighborLists.size(), neighborLists2.size());

	for (size_t i = 0; i < neighborLists.size(); ++i)
	{
		const auto& neighbors = neighborLists[i];
		const auto& neighbors2 = neighborLists2[i];
		EXPECT_EQ(neighbors.size(), neighbors2.size());

		for (size_t j = 0; j < neighbors.size(); ++j)
		{
			EXPECT_EQ(neighbors[j], neighbors2[j]);
		}
	}
}