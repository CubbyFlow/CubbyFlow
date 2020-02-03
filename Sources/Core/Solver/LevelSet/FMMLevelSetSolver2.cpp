/*************************************************************************
> File Name: FMMLevelSetSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Two-dimensional fast marching method (FMM) implementation.
> Created Time: 2017/09/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/FDM/FDMUtils.hpp>
#include <Core/LevelSet/LevelSetUtils.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver2.h>

#include <queue>

namespace CubbyFlow
{
	static const char UNKNOWN = 0;
	static const char KNOWN = 1;
	static const char TRIAL = 2;

	// Find geometric solution near the boundary
	inline double SolveQuadNearBoundary(
		const Array2<char>& markers,
		ArrayAccessor2<double> output,
		const Vector2D& gridSpacing,
		const Vector2D& invGridSpacingSqr,
		double sign,
		size_t i, size_t j)
	{
		UNUSED_VARIABLE(markers);
		UNUSED_VARIABLE(invGridSpacingSqr);

		Size2 size = output.size();

		bool hasX = false;
		double phiX = std::numeric_limits<double>::max();

		if (i > 0)
		{
			if (IsInsideSDF(sign * output(i - 1, j)))
			{
				hasX = true;
				phiX = std::min(phiX, sign * output(i - 1, j));
			}
		}

		if (i + 1 < size.x)
		{
			if (IsInsideSDF(sign * output(i + 1, j)))
			{
				hasX = true;
				phiX = std::min(phiX, sign * output(i + 1, j));
			}
		}

		bool hasY = false;
		double phiY = std::numeric_limits<double>::max();

		if (j > 0)
		{
			if (IsInsideSDF(sign * output(i, j - 1)))
			{
				hasY = true;
				phiY = std::min(phiY, sign * output(i, j - 1));
			}
		}

		if (j + 1 < size.y)
		{
			if (IsInsideSDF(sign * output(i, j + 1)))
			{
				hasY = true;
				phiY = std::min(phiY, sign * output(i, j + 1));
			}
		}

		assert(hasX || hasY);

		double distToBndX = gridSpacing.x * std::abs(output(i, j)) / (std::abs(output(i, j)) + std::abs(phiX));
		double distToBndY = gridSpacing.y * std::abs(output(i, j)) / (std::abs(output(i, j)) + std::abs(phiY));

		double denomSqr = 0.0;

		if (hasX)
		{
			denomSqr += 1.0 / Square(distToBndX);
		}
		if (hasY)
		{
			denomSqr += 1.0 / Square(distToBndY);
		}

		double solution = 1.0 / std::sqrt(denomSqr);

		return sign * solution;
	}

	inline double SolveQuad(
		const Array2<char>& markers,
		ArrayAccessor2<double> output,
		const Vector2D& gridSpacing,
		const Vector2D& invGridSpacingSqr,
		size_t i, size_t j)
	{
		Size2 size = output.size();

		bool hasX = false;
		double phiX = std::numeric_limits<double>::max();

		if (i > 0)
		{
			if (markers(i - 1, j) == KNOWN)
			{
				hasX = true;
				phiX = std::min(phiX, output(i - 1, j));
			}
		}

		if (i + 1 < size.x)
		{
			if (markers(i + 1, j) == KNOWN)
			{
				hasX = true;
				phiX = std::min(phiX, output(i + 1, j));
			}
		}

		bool hasY = false;
		double phiY = std::numeric_limits<double>::max();

		if (j > 0)
		{
			if (markers(i, j - 1) == KNOWN)
			{
				hasY = true;
				phiY = std::min(phiY, output(i, j - 1));
			}
		}

		if (j + 1 < size.y)
		{
			if (markers(i, j + 1) == KNOWN)
			{
				hasY = true;
				phiY = std::min(phiY, output(i, j + 1));
			}
		}

		assert(hasX || hasY);

		double solution = 0.0;

		// Initial guess
		if (hasX)
		{
			solution = phiX + gridSpacing.x;
		}
		if (hasY)
		{
			solution = std::max(solution, phiY + gridSpacing.y);
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

		double det = b * b - a * c;

		if (det > 0.0)
		{
			solution = (-b + std::sqrt(det)) / a;
		}

		return solution;
	}

	FMMLevelSetSolver2::FMMLevelSetSolver2()
	{
		// Do nothing
	}

	void FMMLevelSetSolver2::Reinitialize(
		const ScalarGrid2& inputSDF,
		double maxDistance,
		ScalarGrid2* outputSDF)
	{
		if (!inputSDF.HasSameShape(*outputSDF))
		{
			throw std::invalid_argument("inputSDF and outputSDF have not same shape.");
		}

		Size2 size = inputSDF.GetDataSize();
		Vector2D gridSpacing = inputSDF.GridSpacing();
		Vector2D invGridSpacing = 1.0 / gridSpacing;
		Vector2D invGridSpacingSqr = invGridSpacing * invGridSpacing;
		Array2<char> markers(size);

		auto output = outputSDF->GetDataAccessor();

		markers.ParallelForEachIndex([&](size_t i, size_t j)
		{
			output(i, j) = inputSDF(i, j);
		});

		// Solve geometrically near the boundary
		markers.ForEachIndex([&](size_t i, size_t j)
		{
			if (!IsInsideSDF(output(i, j)) &&
				((i > 0 && IsInsideSDF(output(i - 1, j))) ||
				(i + 1 < size.x && IsInsideSDF(output(i + 1, j))) ||
				(j > 0 && IsInsideSDF(output(i, j - 1))) ||
				(j + 1 < size.y && IsInsideSDF(output(i, j + 1)))))
			{
				output(i, j) = SolveQuadNearBoundary(markers, output, gridSpacing, invGridSpacingSqr, 1.0, i, j);
			}
			else if (IsInsideSDF(output(i, j)) &&
				((i > 0 && !IsInsideSDF(output(i - 1, j))) ||
				(i + 1 < size.x && !IsInsideSDF(output(i + 1, j))) ||
				(j > 0 && !IsInsideSDF(output(i, j - 1))) ||
				(j + 1 < size.y && !IsInsideSDF(output(i, j + 1)))))
			{
				output(i, j) = SolveQuadNearBoundary(markers, output, gridSpacing, invGridSpacingSqr, -1.0, i, j);
			}
		});

		for (int sign = 0; sign < 2; ++sign)
		{
			// Build markers
			markers.ParallelForEachIndex([&](size_t i, size_t j)
			{
				if (IsInsideSDF(output(i, j)))
				{
					markers(i, j) = KNOWN;
				}
				else
				{
					markers(i, j) = UNKNOWN;
				}
			});

			auto compare = [&](const Point2UI& a, const Point2UI& b)
			{
				return output(a.x, a.y) > output(b.x, b.y);
			};

			// Enqueue initial candidates
			std::priority_queue<Point2UI, std::vector<Point2UI>, decltype(compare)> trial(compare);
			markers.ForEachIndex([&](size_t i, size_t j)
			{
				if (markers(i, j) != KNOWN &&
					((i > 0 && markers(i - 1, j) == KNOWN) ||
					(i + 1 < size.x && markers(i + 1, j) == KNOWN) ||
					(j > 0 && markers(i, j - 1) == KNOWN) ||
					(j + 1 < size.y && markers(i, j + 1) == KNOWN)))
				{
					trial.push(Point2UI(i, j));
					markers(i, j) = TRIAL;
				}
			});

			// Propagate
			while (!trial.empty())
			{
				Point2UI idx = trial.top();
				trial.pop();

				size_t i = idx.x;
				size_t j = idx.y;

				markers(i, j) = KNOWN;
				output(i, j) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j);

				if (output(i, j) > maxDistance)
				{
					break;
				}

				if (i > 0)
				{
					if (markers(i - 1, j) == UNKNOWN)
					{
						markers(i - 1, j) = TRIAL;
						output(i - 1, j) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i - 1, j);
						trial.push(Point2UI(i - 1, j));
					}
				}

				if (i + 1 < size.x)
				{
					if (markers(i + 1, j) == UNKNOWN)
					{
						markers(i + 1, j) = TRIAL;
						output(i + 1, j) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i + 1, j);
						trial.push(Point2UI(i + 1, j));
					}
				}

				if (j > 0)
				{
					if (markers(i, j - 1) == UNKNOWN)
					{
						markers(i, j - 1) = TRIAL;
						output(i, j - 1) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j - 1);
						trial.push(Point2UI(i, j - 1));
					}
				}

				if (j + 1 < size.y)
				{
					if (markers(i, j + 1) == UNKNOWN)
					{
						markers(i, j + 1) = TRIAL;
						output(i, j + 1) = SolveQuad(markers, output, gridSpacing, invGridSpacingSqr, i, j + 1);
						trial.push(Point2UI(i, j + 1));
					}
				}
			}

			// Flip the sign
			markers.ParallelForEachIndex([&](size_t i, size_t j)
			{
				output(i, j) = -output(i, j);
			});
		}
	}

	void FMMLevelSetSolver2::Extrapolate(
		const ScalarGrid2& input,
		const ScalarField2& sdf,
		double maxDistance,
		ScalarGrid2* output)
	{
		if (!input.HasSameShape(*output))
		{
			throw std::invalid_argument("input and output have not same shape.");
		}

		Array2<double> sdfGrid(input.GetDataSize());
		auto pos = input.GetDataPosition();
		sdfGrid.ParallelForEachIndex([&](size_t i, size_t j)
		{
			sdfGrid(i, j) = sdf.Sample(pos(i, j));
		});

		Extrapolate(
			input.GetConstDataAccessor(),
			sdfGrid.ConstAccessor(),
			input.GridSpacing(),
			maxDistance,
			output->GetDataAccessor());
	}

	void FMMLevelSetSolver2::Extrapolate(
		const CollocatedVectorGrid2& input,
		const ScalarField2& sdf,
		double maxDistance,
		CollocatedVectorGrid2* output)
	{
		if (!input.HasSameShape(*output))
		{
			throw std::invalid_argument("input and output have not same shape.");
		}

		Array2<double> sdfGrid(input.GetDataSize());
		auto pos = input.GetDataPosition();
		sdfGrid.ParallelForEachIndex([&](size_t i, size_t j)
		{
			sdfGrid(i, j) = sdf.Sample(pos(i, j));
		});

		const Vector2D gridSpacing = input.GridSpacing();

		Array2<double> u(input.GetDataSize());
		Array2<double> u0(input.GetDataSize());
		Array2<double> v(input.GetDataSize());
		Array2<double> v0(input.GetDataSize());

		input.ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			u(i, j) = input(i, j).x;
			v(i, j) = input(i, j).y;
		});

		Extrapolate(u, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, u0);
		Extrapolate(v, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, v0);

		output->ParallelForEachDataPointIndex([&](size_t i, size_t j)
		{
			(*output)(i, j).x = u(i, j);
			(*output)(i, j).y = v(i, j);
		});
	}

	void FMMLevelSetSolver2::Extrapolate(
		const FaceCenteredGrid2& input,
		const ScalarField2& sdf,
		double maxDistance,
		FaceCenteredGrid2* output)
	{
		if (!input.HasSameShape(*output))
		{
			throw std::invalid_argument("inputSDF and outputSDF have not same shape.");
		}

		const Vector2D gridSpacing = input.GridSpacing();

		auto u = input.GetUConstAccessor();
		auto uPos = input.GetUPosition();
		Array2<double> sdfAtU(u.size());
		input.ParallelForEachUIndex([&](size_t i, size_t j)
		{
			sdfAtU(i, j) = sdf.Sample(uPos(i, j));
		});

		Extrapolate(u, sdfAtU, gridSpacing, maxDistance, output->GetUAccessor());

		auto v = input.GetVConstAccessor();
		auto vPos = input.GetVPosition();
		Array2<double> sdfAtV(v.size());
		input.ParallelForEachVIndex([&](size_t i, size_t j)
		{
			sdfAtV(i, j) = sdf.Sample(vPos(i, j));
		});

		Extrapolate(v, sdfAtV, gridSpacing, maxDistance, output->GetVAccessor());
	}

	void FMMLevelSetSolver2::Extrapolate(
		const ConstArrayAccessor2<double>& input,
		const ConstArrayAccessor2<double>& sdf,
		const Vector2D& gridSpacing,
		double maxDistance,
		ArrayAccessor2<double> output)
	{
		Size2 size = input.size();
		Vector2D invGridSpacing = 1.0 / gridSpacing;

		// Build markers
		Array2<char> markers(size, UNKNOWN);
		markers.ParallelForEachIndex([&](size_t i, size_t j)
		{
			if (IsInsideSDF(sdf(i, j)))
			{
				markers(i, j) = KNOWN;
			}
			output(i, j) = input(i, j);
		});

		auto compare = [&](const Point2UI& a, const Point2UI& b)
		{
			return sdf(a.x, a.y) > sdf(b.x, b.y);
		};

		// Enqueue initial candidates
		std::priority_queue<Point2UI, std::vector<Point2UI>, decltype(compare)> trial(compare);
		markers.ForEachIndex([&](size_t i, size_t j)
		{
			if (markers(i, j) == KNOWN)
			{
				return;
			}

			if (i > 0 && markers(i - 1, j) == KNOWN)
			{
				trial.push(Point2UI(i, j));
				markers(i, j) = TRIAL;
				return;
			}

			if (i + 1 < size.x && markers(i + 1, j) == KNOWN)
			{
				trial.push(Point2UI(i, j));
				markers(i, j) = TRIAL;
				return;
			}

			if (j > 0 && markers(i, j - 1) == KNOWN)
			{
				trial.push(Point2UI(i, j));
				markers(i, j) = TRIAL;
				return;
			}

			if (j + 1 < size.y && markers(i, j + 1) == KNOWN)
			{
				trial.push(Point2UI(i, j));
				markers(i, j) = TRIAL;
				return;
			}
		});

		// Propagate
		while (!trial.empty())
		{
			Point2UI idx = trial.top();
			trial.pop();

			size_t i = idx.x;
			size_t j = idx.y;

			if (sdf(i, j) > maxDistance)
			{
				break;
			}

			Vector2D grad = Gradient2(sdf, gridSpacing, i, j).Normalized();

			double sum = 0.0;
			double count = 0.0;

			if (i > 0)
			{
				if (markers(i - 1, j) == KNOWN)
				{
					double weight = std::max(grad.x, 0.0) * invGridSpacing.x;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i - 1, j);
					count += weight;
				}
				else if (markers(i - 1, j) == UNKNOWN)
				{
					markers(i - 1, j) = TRIAL;
					trial.push(Point2UI(i - 1, j));
				}
			}

			if (i + 1 < size.x)
			{
				if (markers(i + 1, j) == KNOWN)
				{
					double weight = -std::min(grad.x, 0.0) * invGridSpacing.x;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i + 1, j);
					count += weight;
				}
				else if (markers(i + 1, j) == UNKNOWN)
				{
					markers(i + 1, j) = TRIAL;
					trial.push(Point2UI(i + 1, j));
				}
			}

			if (j > 0)
			{
				if (markers(i, j - 1) == KNOWN)
				{
					double weight = std::max(grad.y, 0.0) * invGridSpacing.y;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i, j - 1);
					count += weight;
				}
				else if (markers(i, j - 1) == UNKNOWN)
				{
					markers(i, j - 1) = TRIAL;
					trial.push(Point2UI(i, j - 1));
				}
			}

			if (j + 1 < size.y)
			{
				if (markers(i, j + 1) == KNOWN)
				{
					double weight = -std::min(grad.y, 0.0) * invGridSpacing.y;

					// If gradient is zero, then just assign 1 to weight
					if (weight < std::numeric_limits<double>::epsilon())
					{
						weight = 1.0;
					}

					sum += weight * output(i, j + 1);
					count += weight;
				}
				else if (markers(i, j + 1) == UNKNOWN)
				{
					markers(i, j + 1) = TRIAL;
					trial.push(Point2UI(i, j + 1));
				}
			}

			assert(count > 0.0);

			output(i, j) = sum / count;
			markers(i, j) = KNOWN;
		}
	}
}