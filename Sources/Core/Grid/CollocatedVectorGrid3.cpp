/*************************************************************************
> File Name: CollocatedVectorGrid3.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 3-D collocated vector grid structure.
> Created Time: 2017/08/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Grid/CollocatedVectorGrid3.h>

namespace CubbyFlow
{
	CollocatedVectorGrid3::CollocatedVectorGrid3() :
		m_linearSampler(m_data.ConstAccessor(), Vector3D(1, 1, 1), Vector3D())
	{
		// Do nothing
	}

	CollocatedVectorGrid3::~CollocatedVectorGrid3()
	{
		// Do nothing
	}

	const Vector3D& CollocatedVectorGrid3::operator()(size_t i, size_t j, size_t k) const
	{
		return m_data(i, j, k);
	}

	Vector3D& CollocatedVectorGrid3::operator()(size_t i, size_t j, size_t k)
	{
		return m_data(i, j, k);
	}

	double CollocatedVectorGrid3::DivergenceAtDataPoint(size_t i, size_t j, size_t k) const
	{
		const Size3 ds = m_data.size();
		const Vector3D& gs = GridSpacing();

		assert(i < ds.x && j < ds.y && k < ds.z);

		double left = m_data((i > 0) ? i - 1 : i, j, k).x;
		double right = m_data((i + 1 < ds.x) ? i + 1 : i, j, k).x;
		double down = m_data(i, (j > 0) ? j - 1 : j, k).y;
		double up = m_data(i, (j + 1 < ds.y) ? j + 1 : j, k).y;
		double back = m_data(i, j, (k > 0) ? k - 1 : k).z;
		double front = m_data(i, j, (k + 1 < ds.z) ? k + 1 : k).z;

		return
			0.5 * (right - left) / gs.x +
			0.5 * (up - down) / gs.y +
			0.5 * (front - back) / gs.z;
	}

	Vector3D CollocatedVectorGrid3::CurlAtDataPoint(size_t i, size_t j, size_t k) const
	{
		const Size3 ds = m_data.size();
		const Vector3D& gs = GridSpacing();

		assert(i < ds.x && j < ds.y && k < ds.z);

		Vector3D left = m_data((i > 0) ? i - 1 : i, j, k);
		Vector3D right = m_data((i + 1 < ds.x) ? i + 1 : i, j, k);
		Vector3D down = m_data(i, (j > 0) ? j - 1 : j, k);
		Vector3D up = m_data(i, (j + 1 < ds.y) ? j + 1 : j, k);
		Vector3D back = m_data(i, j, (k > 0) ? k - 1 : k);
		Vector3D front = m_data(i, j, (k + 1 < ds.z) ? k + 1 : k);

		double Fx_ym = down.x;
		double Fx_yp = up.x;
		double Fx_zm = back.x;
		double Fx_zp = front.x;

		double Fy_xm = left.y;
		double Fy_xp = right.y;
		double Fy_zm = back.y;
		double Fy_zp = front.y;

		double Fz_xm = left.z;
		double Fz_xp = right.z;
		double Fz_ym = down.z;
		double Fz_yp = up.z;

		return Vector3D(
			0.5 * (Fz_yp - Fz_ym) / gs.y - 0.5 * (Fy_zp - Fy_zm) / gs.z,
			0.5 * (Fx_zp - Fx_zm) / gs.z - 0.5 * (Fz_xp - Fz_xm) / gs.x,
			0.5 * (Fy_xp - Fy_xm) / gs.x - 0.5 * (Fx_yp - Fx_ym) / gs.y);
	}

	Vector3D CollocatedVectorGrid3::Sample(const Vector3D& x) const
	{
		return m_sampler(x);
	}

	double CollocatedVectorGrid3::Divergence(const Vector3D& x) const
	{
		std::array<Point3UI, 8> indices;
		std::array<double, 8> weights;
		m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

		double result = 0.0;
		for (int i = 0; i < 8; ++i)
		{
			result += weights[i] * DivergenceAtDataPoint(indices[i].x, indices[i].y, indices[i].z);
		}

		return result;
	}

	Vector3D CollocatedVectorGrid3::Curl(const Vector3D& x) const
	{
		std::array<Point3UI, 8> indices;
		std::array<double, 8> weights;
		m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

		Vector3D result;
		for (int i = 0; i < 8; ++i)
		{
			result += weights[i] * CurlAtDataPoint(indices[i].x, indices[i].y, indices[i].z);
		}

		return result;
	}

	std::function<Vector3D(const Vector3D&)> CollocatedVectorGrid3::Sampler() const
	{
		return m_sampler;
	}

	VectorGrid3::VectorDataAccessor CollocatedVectorGrid3::GetDataAccessor()
	{
		return m_data.Accessor();
	}

	VectorGrid3::ConstVectorDataAccessor CollocatedVectorGrid3::GetConstDataAccessor() const
	{
		return m_data.ConstAccessor();
	}

	VectorGrid3::DataPositionFunc CollocatedVectorGrid3::GetDataPosition() const
	{
		Vector3D dataOrigin = GetDataOrigin();
		return [this, dataOrigin](size_t i, size_t j, size_t k) -> Vector3D
		{
			return dataOrigin + GridSpacing() * Vector3D({ i, j, k });
		};
	}

	void CollocatedVectorGrid3::ForEachDataPointIndex(const std::function<void(size_t, size_t, size_t)>& func) const
	{
		m_data.ForEachIndex(func);
	}

	void CollocatedVectorGrid3::ParallelForEachDataPointIndex(const std::function<void(size_t, size_t, size_t)>& func) const
	{
		m_data.ParallelForEachIndex(func);
	}

	void CollocatedVectorGrid3::SwapCollocatedVectorGrid(CollocatedVectorGrid3* other)
	{
		SwapGrid(other);

		m_data.Swap(other->m_data);
		std::swap(m_linearSampler, other->m_linearSampler);
		std::swap(m_sampler, other->m_sampler);
	}

	void CollocatedVectorGrid3::SetCollocatedVectorGrid(const CollocatedVectorGrid3& other)
	{
		SetGrid(other);

		m_data.Set(other.m_data);
		ResetSampler();
	}

	void CollocatedVectorGrid3::OnResize(
		const Size3& resolution,
		const Vector3D& gridSpacing,
		const Vector3D& origin,
		const Vector3D& initialValue)
	{
		UNUSED_VARIABLE(resolution);
		UNUSED_VARIABLE(gridSpacing);
		UNUSED_VARIABLE(origin);

		m_data.Resize(GetDataSize(), initialValue);
		ResetSampler();
	}

	void CollocatedVectorGrid3::ResetSampler()
	{
		m_linearSampler = LinearArraySampler3<Vector3D, double>(m_data.ConstAccessor(), GridSpacing(), GetDataOrigin());
		m_sampler = m_linearSampler.Functor();
	}

	void CollocatedVectorGrid3::GetData(std::vector<double>* data) const
	{
		size_t size = 3 * GetDataSize().x * GetDataSize().y * GetDataSize().z;
		data->resize(size);
		size_t cnt = 0;

		m_data.ForEach([&](const Vector3D& value)
		{
			(*data)[cnt++] = value.x;
			(*data)[cnt++] = value.y;
			(*data)[cnt++] = value.z;
		});
	}

	void CollocatedVectorGrid3::SetData(const std::vector<double>& data)
	{
		assert(3 * GetDataSize().x * GetDataSize().y * GetDataSize().z == data.size());

		size_t cnt = 0;

		m_data.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			m_data(i, j, k).x = data[cnt++];
			m_data(i, j, k).y = data[cnt++];
			m_data(i, j, k).z = data[cnt++];
		});
	}
}