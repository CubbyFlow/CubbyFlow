/*************************************************************************
> File Name: FDMMGLinearSystem2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Multigrid-syle 2-D linear system.
> Created Time: 2017/11/02
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/FDM/FDMMGLinearSystem2.h>

namespace CubbyFlow
{
	void FDMMGLinearSystem2::Clear()
	{
		A.levels.clear();
		x.levels.clear();
		b.levels.clear();
	}

	size_t FDMMGLinearSystem2::GetNumberOfLevels() const
	{
		return A.levels.size();
	}

	void FDMMGLinearSystem2::ResizeWithCoarsest(const Size2& coarsestResolution, size_t numberOfLevels)
	{
		FDMMGUtils2::ResizeArrayWithCoarsest(coarsestResolution, numberOfLevels, &A.levels);
		FDMMGUtils2::ResizeArrayWithCoarsest(coarsestResolution, numberOfLevels, &x.levels);
		FDMMGUtils2::ResizeArrayWithCoarsest(coarsestResolution, numberOfLevels, &b.levels);
	}

	void FDMMGLinearSystem2::ResizeWithFinest(const Size2& finestResolution, size_t maxNumberOfLevels)
	{
		FDMMGUtils2::ResizeArrayWithFinest(finestResolution, maxNumberOfLevels, &A.levels);
		FDMMGUtils2::ResizeArrayWithFinest(finestResolution, maxNumberOfLevels, &x.levels);
		FDMMGUtils2::ResizeArrayWithFinest(finestResolution, maxNumberOfLevels, &b.levels);
	}

	void FDMMGUtils2::Restrict(const FDMVector2& finer, FDMVector2* coarser)
	{
		assert(finer.size().x == 2 * coarser->size().x);
		assert(finer.size().y == 2 * coarser->size().y);

		// --*--|--*--|--*--|--*--
		//  1/8   3/8   3/8   1/8
		//           to
		// -----|-----*-----|-----
		static const std::array<double, 4> kernel = { { 0.125, 0.375, 0.375, 0.125 } };

		const Size2 n = coarser->size();
		ParallelRangeFor(ZERO_SIZE, n.x, ZERO_SIZE, n.y,
			[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd)
		{
			std::array<size_t, 4> jIndices;

			for (size_t j = jBegin; j < jEnd; ++j)
			{
				jIndices[0] = (j > 0) ? 2 * j - 1 : 2 * j;
				jIndices[1] = 2 * j;
				jIndices[2] = 2 * j + 1;
				jIndices[3] = (j + 1 < n.y) ? 2 * j + 2 : 2 * j + 1;

				std::array<size_t, 4> iIndices;
				for (size_t i = iBegin; i < iEnd; ++i)
				{
					iIndices[0] = (i > 0) ? 2 * i - 1 : 2 * i;
					iIndices[1] = 2 * i;
					iIndices[2] = 2 * i + 1;
					iIndices[3] = (i + 1 < n.x) ? 2 * i + 2 : 2 * i + 1;

					double sum = 0.0;
					for (size_t y = 0; y < 4; ++y)
					{
						for (size_t x = 0; x < 4; ++x)
						{
							double w = kernel[x] * kernel[y];
							sum += w * finer(iIndices[x], jIndices[y]);
						}
					}

					(*coarser)(i, j) = sum;
				}
			}
		});
	}

	void FDMMGUtils2::Correct(const FDMVector2& coarser, FDMVector2* finer)
	{
		assert(finer->size().x == 2 * coarser.size().x);
		assert(finer->size().y == 2 * coarser.size().y);

		// -----|-----*-----|-----
		//           to
		//  1/4   3/4   3/4   1/4
		// --*--|--*--|--*--|--*--
		const Size2 n = finer->size();
		ParallelRangeFor(ZERO_SIZE, n.x, ZERO_SIZE, n.y,
			[&](size_t iBegin, size_t iEnd, size_t jBegin, size_t jEnd)
		{
			for (size_t j = jBegin; j < jEnd; ++j)
			{
				for (size_t i = iBegin; i < iEnd; ++i)
				{
					std::array<size_t, 2> iIndices;
					std::array<size_t, 2> jIndices;
					std::array<double, 2> iWeights;
					std::array<double, 2> jWeights;

					const size_t ci = i / 2;
					const size_t cj = j / 2;

					if (i % 2 == 0)
					{
						iIndices[0] = (i > 1) ? ci - 1 : ci;
						iIndices[1] = ci;
						iWeights[0] = 0.25;
						iWeights[1] = 0.75;
					}
					else
					{
						iIndices[0] = ci;
						iIndices[1] = (i + 1 < n.x) ? ci + 1 : ci;
						iWeights[0] = 0.75;
						iWeights[1] = 0.25;
					}

					if (j % 2 == 0)
					{
						jIndices[0] = (j > 1) ? cj - 1 : cj;
						jIndices[1] = cj;
						jWeights[0] = 0.25;
						jWeights[1] = 0.75;
					}
					else
					{
						jIndices[0] = cj;
						jIndices[1] = (j + 1 < n.y) ? cj + 1 : cj;
						jWeights[0] = 0.75;
						jWeights[1] = 0.25;
					}

					for (size_t y = 0; y < 2; ++y)
					{
						for (size_t x = 0; x < 2; ++x)
						{
							double w = iWeights[x] * jWeights[y] * coarser(iIndices[x], jIndices[y]);
							(*finer)(i, j) += w;
						}
					}
				}
			}
		});
	}
}