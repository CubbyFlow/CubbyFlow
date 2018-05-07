/*************************************************************************
> File Name: FMMLevelSetSolver3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Three-dimensional fast marching method (FMM) implementation.
> Created Time: 2017/09/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/FDM/FDMUtils.h>
#include <Core/LevelSet/LevelSetUtils.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.h>

#include <queue>

namespace CubbyFlow
{
	static const char UNKNOWN = 0;
	static const char KNOWN = 1;
	static const char TRIAL = 2;

	// Find geometric solution near the boundary
	inline double SolveQuadNearBoundary(
		const Array3<char>& markers,
		ArrayAccessor3<double> output,
		const Vector3D& gridSpacing,
		const Vector3D& invGridSpacingSqr,
		double sign,
		size_t i, size_t j, size_t k)
	{
		UNUSED_VARIABLE(markers);
		UNUSED_VARIABLE(invGridSpacingSqr);

		Size3 size = output.size();

		bool hasX = false;
		double phiX = std::numeric_limits<double>::max();

		if (i > 0)
		{
			if (IsInsideSDF(sign * output(i - 1, j, k)))
			{
				hasX = true;
				phiX = std::min(phiX, sign * output(i - 1, j, k));
			}
		}

		if (i + 1 < size.x)
		{
			if (IsInsideSDF(sign * output(i + 1, j, k)))
			{
				hasX = true;
				phiX = std::min(phiX, sign * output(i + 1, j, k));
			}
		}

		bool hasY = false;
		double phiY = std::numeric_limits<double>::max();

		if (j > 0)
		{
			if (IsInsideSDF(sign * output(i, j - 1, k)))
			{
				hasY = true;
				phiY = std::min(phiY, sign * output(i, j - 1, k));
			}
		}

		if (j + 1 < size.y)
		{
			if (IsInsideSDF(sign * output(i, j + 1, k)))
			{
				hasY = true;
				phiY = std::min(phiY, sign * output(i, j + 1, k));
			}
		}

		bool hasZ = false;
		double phiZ = std::numeric_limits<double>::max();

		if (k > 0)
		{
			if (IsInsideSDF(sign * output(i, j, k - 1)))
			{
				hasZ = true;
				phiZ = std::min(phiZ, sign * output(i, j, k - 1));
			}
		}

		if (k + 1 < size.z)
		{
			if (IsInsideSDF(sign * output(i, j, k + 1)))
			{
				hasZ = true;
				phiZ = std::min(phiZ, sign * output(i, j, k + 1));
			}
		}

		assert(hasX || hasY || hasZ);

		double distToBndX
			= gridSpacing.x * std::abs(output(i, j, k))
			/ (std::abs(output(i, j, k)) + std::abs(phiX));

		double distToBndY
			= gridSpacing.y * std::abs(output(i, j, k))
			/ (std::abs(output(i, j, k)) + std::abs(phiY));

		double distToBndZ
			= gridSpacing.z * std::abs(output(i, j, k))
			/ (std::abs(output(i, j, k)) + std::abs(phiZ));

		double denomSqr = 0.0;

		if (hasX)
		{
			denomSqr += 1.0 / Square(distToBndX);
		}
		if (hasY)
		{
			denomSqr += 1.0 / Square(distToBndY);
		}
		if (hasZ)
		{
			denomSqr += 1.0 / Square(distToBndZ);
		}

		double solution = 1.0 / std::sqrt(denomSqr);

		return sign * solution;
	}

	inline double SolveQuad(
		const Array3<char>& markers,
		ArrayAccessor3<double> output,
		const Vector3D& gridSpacing,
		const Vector3D& invGridSpacingSqr,
		size_t i, size_t j, size_t k)
	{
		Size3 size = output.size();

		bool hasX = false;
		double phiX = std::numeric_limits<double>::max();

		if (i > 0)
		{
			if (markers(i - 1, j, k) == KNOWN)
			{
				hasX = true;
				phiX = std::min(phiX, output(i - 1, j, k));
			}
		}

		if (i + 1 < size.x)
		{
			if (markers(i + 1, j, k) == KNOWN)
			{
				hasX = true;
				phiX = std::min(phiX, output(i + 1, j, k));
			}
		}

		bool hasY = false;
		double phiY = std::numeric_limits<double>::max();

		if (j > 0)
		{
			if (markers(i, j - 1, k) == KNOWN)
			{
				hasY = true;
				phiY = std::min(phiY, output(i, j - 1, k));
			}
		}

		if (j + 1 < size.y)
		{
			if (markers(i, j + 1, k) == KNOWN)
			{
				hasY = true;
				phiY = std::min(phiY, output(i, j + 1, k));
			}
		}

		bool hasZ = false;
		double phiZ = std::numeric_limits<double>::max();

		if (k > 0)
		{
			if (markers(i, j, k - 1) == KNOWN)
			{
				hasZ = true;
				phiZ = std::min(phiZ, output(i, j, k - 1));
			}
		}

		if (k + 1 < size.z)
		{
			if (markers(i, j, k + 1) == KNOWN)
			{
				hasZ = true;
				phiZ = std::min(phiZ, output(i, j, k + 1));
			}
		}

		assert(hasX || hasY || hasZ);

		double solution = 0.0;

		// Initial guess
		if (hasX)
		{
			solution = std::max(solution, phiX + gridSpacing.x);
		}
		if (hasY)
		{
			solution = std::max(solution, phiY + gridSpacing.y);
		}
		if (hasZ)
		{
			solution = std::max(solution, phiZ + gridSpacing.z);
		}

		// Solve quad
		double a = 0.0;
		double b = 0.0;
		double c = -1.0;

		if (hasX)
		{
			a += invGridSpacingSqr.x;
			b -= phiX * invGridSpacingSqr.x;
			c += Square(phiX) * invGridSpacingSqr.x;
		}
		if (hasY)
		{
			a += invGridSpacingSqr.y;
			b -= phiY * invGridSpacingSqr.y;
			c += Square(phiY) * invGridSpacingSqr.y;
		}
		if (hasZ)
		{
			a += invGridSpacingSqr.z;
			b -= phiZ * invGridSpacingSqr.z;
			c += Square(phiZ) * invGridSpacingSqr.z;
		}

		double det = b * b - a * c;

		if (det > 0.0)
		{
			solution = (-b + std::sqrt(det)) / a;
		}

		return solution;
	}

	FMMLevelSetSolver3::FMMLevelSetSolver3()
	{
		// Do nothing
	}

	void FMMLevelSetSolver3::Reinitialize(
		const ScalarGrid3& inputSDF,
		double maxDistance,
		ScalarGrid3* outputSDF)
	{
		if (!inputSDF.HasSameShape(*outputSDF))
		{
			throw std::invalid_argument("inputSDF and outputSDF have not same shape.");
		}

		Size3 size = inputSDF.GetDataSize();
		Vector3D gridSpacing = inputSDF.GridSpacing();
		Vector3D invGridSpacing = 1.0 / gridSpacing;
		Vector3D invGridSpacingSqr = invGridSpacing * invGridSpacing;
		Array3<char> markers(size);

		auto output = outputSDF->GetDataAccessor();

		markers.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			output(i, j, k) = inputSDF(i, j, k);
		});

		// Solve geometrically near the boundary
		markers.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (!IsInsideSDF(output(i, j, k)) &&
				((i > 0 && IsInsideSDF(output(i - 1, j, k))) ||
				(i + 1 < size.x && IsInsideSDF(output(i + 1, j, k))) ||
				(j > 0 && IsInsideSDF(output(i, j - 1, k))) ||
				(j + 1 < size.y && IsInsideSDF(output(i, j + 1, k))) ||
				(k > 0 && IsInsideSDF(output(i, j, k - 1))) ||
				(k + 1 < size.z && IsInsideSDF(output(i, j, k + 1)))))
			{
				output(i, j, k) = SolveQuadNearBoundary(markers, output, gridSpacing, invGridSpacingSqr, 1.0, i, j, k);
			}
			else if (IsInsideSDF(output(i, j, k)) &&
				((i > 0 && !IsInsideSDF(output(i - 1, j, k))) ||
				(i + 1 < size.x && !IsInsideSDF(output(i + 1, j, k))) ||
				(j > 0 && !IsInsideSDF(output(i, j - 1, k))) ||
				(j + 1 < size.y && !IsInsideSDF(output(i, j + 1, k))) ||
				(k > 0 && !IsInsideSDF(output(i, j, k - 1))) ||
				(k + 1 < size.z && !IsInsideSDF(output(i, j, k + 1)))))
			{
				output(i, j, k) = SolveQuadNearBoundary(markers, output, gridSpacing, invGridSpacingSqr, -1.0, i, j, k);
			}
		});

		for (int sign = 0; sign < 2; ++sign)
		{
			// Build markers
			markers.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
			{
				if (IsInsideSDF(output(i, j, k)))
				{
					markers(i, j, k) = KNOWN;
				}
				else
				{
					markers(i, j, k) = UNKNOWN;
				}
			});

			auto compare = [&](const Point3UI& a, const Point3UI& b)
			{
				return output(a.x, a.y, a.z) > output(b.x, b.y, b.z);
			};

			// Enqueue initial candidates
			std::priority_queue<Point3UI, std::vector<Point3UI>, decltype(compare)> trial(compare);
			markers.ForEachIndex([&](size_t i, size_t j, size_t k)
			{
				if (markers(i, j, k) != KNOWN &&
					((i > 0 && markers(i - 1, j, k) == KNOWN) ||
					(i + 1 < size.x && markers(i + 1, j, k) == KNOWN) ||
					(j > 0 && markers(i, j - 1, k) == KNOWN) ||
					(j + 1 < size.y && markers(i, j + 1, k) == KNOWN) ||
					(k > 0 && markers(i, j, k - 1) == KNOWN) ||
					(k + 1 < size.z && markers(i, j, k + 1) == KNOWN)))
				{
					trial.push(Point3UI(i, j, k));
					markers(i, j, k) = TRIAL;
				}
			});

			// Propagate
			while (!trial.empty())
			{
				Point3UI idx = trial.top();
				trial.pop();

				size_t i = idx.x;
				size_t j = idx.y;
				size_t k = idx.z;

				markers(i, j, k) = KNOWN;
				output(i, j, k) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j, k);

				if (output(i, j, k) > maxDistance)
				{
					break;
				}

				if (i > 0)
				{
					if (markers(i - 1, j, k) == UNKNOWN)
					{
						markers(i - 1, j, k) = TRIAL;
						output(i - 1, j, k) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i - 1, j, k);
						trial.push(Point3UI(i - 1, j, k));
					}
				}

				if (i + 1 < size.x)
				{
					if (markers(i + 1, j, k) == UNKNOWN)
					{
						markers(i + 1, j, k) = TRIAL;
						output(i + 1, j, k) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i + 1, j, k);
						trial.push(Point3UI(i + 1, j, k));
					}
				}

				if (j > 0)
				{
					if (markers(i, j - 1, k) == UNKNOWN)
					{
						markers(i, j - 1, k) = TRIAL;
						output(i, j - 1, k) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j - 1, k);
						trial.push(Point3UI(i, j - 1, k));
					}
				}

				if (j + 1 < size.y)
				{
					if (markers(i, j + 1, k) == UNKNOWN)
					{
						markers(i, j + 1, k) = TRIAL;
						output(i, j + 1, k) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j + 1, k);
						trial.push(Point3UI(i, j + 1, k));
					}
				}

				if (k > 0)
				{
					if (markers(i, j, k - 1) == UNKNOWN)
					{
						markers(i, j, k - 1) = TRIAL;
						output(i, j, k - 1) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j, k - 1);
						trial.push(Point3UI(i, j, k - 1));
					}
				}

				if (k + 1 < size.z)
				{
					if (markers(i, j, k + 1) == UNKNOWN)
					{
						markers(i, j, k + 1) = TRIAL;
						output(i, j, k + 1) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j, k + 1);
						trial.push(Point3UI(i, j, k + 1));
					}
				}
			}

			// Flip the sign
			markers.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
			{
				output(i, j, k) = -output(i, j, k);
			});
		}
	}

	void FMMLevelSetSolver3::Extrapolate(
		const ScalarGrid3& input,
		const ScalarField3& sdf,
		double maxDistance,
		ScalarGrid3* output)
	{
		if (!input.HasSameShape(*output))
		{
			throw std::invalid_argument("input and output have not same shape.");
		}

		Array3<double> sdfGrid(input.GetDataSize());
		auto pos = input.GetDataPosition();
		sdfGrid.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			sdfGrid(i, j, k) = sdf.Sample(pos(i, j, k));
		});

		Extrapolate(
			input.GetConstDataAccessor(),
			sdfGrid.ConstAccessor(),
			input.GridSpacing(),
			maxDistance,
			output->GetDataAccessor());
	}

	void FMMLevelSetSolver3::Extrapolate(
		const CollocatedVectorGrid3& input,
		const ScalarField3& sdf,
		double maxDistance,
		CollocatedVectorGrid3* output)
	{
		if (!input.HasSameShape(*output))
		{
			throw std::invalid_argument("input and output have not same shape.");
		}

		Array3<double> sdfGrid(input.GetDataSize());
		auto pos = input.GetDataPosition();
		sdfGrid.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			sdfGrid(i, j, k) = sdf.Sample(pos(i, j, k));
		});

		const Vector3D gridSpacing = input.GridSpacing();

		Array3<double> u(input.GetDataSize());
		Array3<double> u0(input.GetDataSize());
		Array3<double> v(input.GetDataSize());
		Array3<double> v0(input.GetDataSize());
		Array3<double> w(input.GetDataSize());
		Array3<double> w0(input.GetDataSize());

		input.ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			u(i, j, k) = input(i, j, k).x;
			v(i, j, k) = input(i, j, k).y;
			w(i, j, k) = input(i, j, k).z;
		});

		Extrapolate(u, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, u0);
		Extrapolate(v, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, v0);
		Extrapolate(w, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, w0);

		output->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
		{
			(*output)(i, j, k).x = u(i, j, k);
			(*output)(i, j, k).y = v(i, j, k);
			(*output)(i, j, k).z = w(i, j, k);
		});
	}

	void FMMLevelSetSolver3::Extrapolate(
		const FaceCenteredGrid3& input,
		const ScalarField3& sdf,
		double maxDistance,
		FaceCenteredGrid3* output)
	{
		if (!input.HasSameShape(*output))
		{
			throw std::invalid_argument("inputSDF and outputSDF have not same shape.");
		}

		const Vector3D gridSpacing = input.GridSpacing();

		auto u = input.GetUConstAccessor();
		auto uPos = input.GetUPosition();
		Array3<double> sdfAtU(u.size());
		input.ParallelForEachUIndex([&](size_t i, size_t j, size_t k)
		{
			sdfAtU(i, j, k) = sdf.Sample(uPos(i, j, k));
		});

		Extrapolate(u, sdfAtU, gridSpacing, maxDistance, output->GetUAccessor());

		auto v = input.GetVConstAccessor();
		auto vPos = input.GetVPosition();
		Array3<double> sdfAtV(v.size());
		input.ParallelForEachVIndex([&](size_t i, size_t j, size_t k)
		{
			sdfAtV(i, j, k) = sdf.Sample(vPos(i, j, k));
		});

		Extrapolate(v, sdfAtV, gridSpacing, maxDistance, output->GetVAccessor());

		auto w = input.GetWConstAccessor();
		auto wPos = input.GetWPosition();
		Array3<double> sdfAtW(w.size());
		input.ParallelForEachWIndex([&](size_t i, size_t j, size_t k)
		{
			sdfAtW(i, j, k) = sdf.Sample(wPos(i, j, k));
		});

		Extrapolate(w, sdfAtW, gridSpacing, maxDistance, output->GetWAccessor());
	}

	void FMMLevelSetSolver3::Extrapolate(
		const ConstArrayAccessor3<double>& input,
		const ConstArrayAccessor3<double>& sdf,
		const Vector3D& gridSpacing,
		double maxDistance,
		ArrayAccessor3<double> output)
	{
		Size3 size = input.size();
		Vector3D invGridSpacing = 1.0 / gridSpacing;

		// Build markers
		Array3<char> markers(size, UNKNOWN);
		markers.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (IsInsideSDF(sdf(i, j, k)))
			{
				markers(i, j, k) = KNOWN;
			}
			output(i, j, k) = input(i, j, k);
		});

		auto compare = [&](const Point3UI& a, const Point3UI& b)
		{
			return sdf(a.x, a.y, a.z) > sdf(b.x, b.y, b.z);
		};

		// Enqueue initial candidates
		std::priority_queue<Point3UI, std::vector<Point3UI>, decltype(compare)> trial(compare);
		markers.ForEachIndex([&](size_t i, size_t j, size_t k)
		{
			if (markers(i, j, k) == KNOWN)
			{
				return;
			}

			if (i > 0 && markers(i - 1, j, k) == KNOWN)
			{
				trial.push(Point3UI(i, j, k));
				markers(i, j, k) = TRIAL;
				return;
			}

			if (i + 1 < size.x && markers(i + 1, j, k) == KNOWN)
			{
				trial.push(Point3UI(i, j, k));
				markers(i, j, k) = TRIAL;
				return;
			}

			if (j > 0 && markers(i, j - 1, k) == KNOWN)
			{
				trial.push(Point3UI(i, j, k));
				markers(i, j, k) = TRIAL;
				return;
			}

			if (j + 1 < size.y && markers(i, j + 1, k) == KNOWN)
			{
				trial.push(Point3UI(i, j, k));
				markers(i, j, k) = TRIAL;
				return;
			}

			if (k > 0 && markers(i, j, k - 1) == KNOWN)
			{
				trial.push(Point3UI(i, j, k));
				markers(i, j, k) = TRIAL;
				return;
			}

			if (k + 1 < size.z && markers(i, j, k + 1) == KNOWN)
			{
				trial.push(Point3UI(i, j, k));
				markers(i, j, k) = TRIAL;
				return;
			}
		});

		// Propagate
		while (!trial.empty())
		{
			Point3UI idx = trial.top();
			trial.pop();

			size_t i = idx.x;
			size_t j = idx.y;
			size_t k = idx.z;

			if (sdf(i, j, k) > maxDistance)
			{
				break;
			}

			Vector3D grad = Gradient3(sdf, gridSpacing, i, j, k).Normalized();

			double sum = 0.0;
			double count = 0.0;

			if (i > 0)
			{
				if (markers(i - 1, j, k) == KNOWN)
				{
					double weight = std::max(grad.x, 0.0) * invGridSpacing.x;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i - 1, j, k);
					count += weight;
				}
				else if (markers(i - 1, j, k) == UNKNOWN)
				{
					markers(i - 1, j, k) = TRIAL;
					trial.push(Point3UI(i - 1, j, k));
				}
			}

			if (i + 1 < size.x)
			{
				if (markers(i + 1, j, k) == KNOWN)
				{
					double weight = -std::min(grad.x, 0.0) * invGridSpacing.x;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i + 1, j, k);
					count += weight;
				}
				else if (markers(i + 1, j, k) == UNKNOWN)
				{
					markers(i + 1, j, k) = TRIAL;
					trial.push(Point3UI(i + 1, j, k));
				}
			}

			if (j > 0)
			{
				if (markers(i, j - 1, k) == KNOWN)
				{
					double weight = std::max(grad.y, 0.0) * invGridSpacing.y;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i, j - 1, k);
					count += weight;
				}
				else if (markers(i, j - 1, k) == UNKNOWN)
				{
					markers(i, j - 1, k) = TRIAL;
					trial.push(Point3UI(i, j - 1, k));
				}
			}

			if (j + 1 < size.y)
			{
				if (markers(i, j + 1, k) == KNOWN)
				{
					double weight = -std::min(grad.y, 0.0) * invGridSpacing.y;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i, j + 1, k);
					count += weight;
				}
				else if (markers(i, j + 1, k) == UNKNOWN)
				{
					markers(i, j + 1, k) = TRIAL;
					trial.push(Point3UI(i, j + 1, k));
				}
			}

			if (k > 0)
			{
				if (markers(i, j, k - 1) == KNOWN)
				{
					double weight = std::max(grad.z, 0.0) * invGridSpacing.z;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i, j, k - 1);
					count += weight;
				}
				else if (markers(i, j, k - 1) == UNKNOWN)
				{
					markers(i, j, k - 1) = TRIAL;
					trial.push(Point3UI(i, j, k - 1));
				}
			}

			if (k + 1 < size.z)
			{
				if (markers(i, j, k + 1) == KNOWN)
				{
					double weight = -std::min(grad.z, 0.0) * invGridSpacing.z;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i, j, k + 1);
					count += weight;
				}
				else if (markers(i, j, k + 1) == UNKNOWN)
				{
					markers(i, j, k + 1) = TRIAL;
					trial.push(Point3UI(i, j, k + 1));
				}
			}

			assert(count > 0.0);

			output(i, j, k) = sum / count;
			markers(i, j, k) = KNOWN;
		}
	}
}