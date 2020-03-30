#ifndef CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER_TEST_HELPER2_H
#define CUBBYFLOW_FDM_LINEAR_SYSTEM_SOLVER_TEST_HELPER2_H

#include <Core/Solver/FDM/FDMLinearSystemSolver2.hpp>

namespace CubbyFlow
{
    class FDMLinearSystemSolverTestHelper2
    {
    public:
        static void BuildTestLinearSystem(FDMLinearSystem2* system, const Size2& size)
        {
            system->A.Resize(size);
            system->x.Resize(size);
            system->b.Resize(size);

            system->A.ForEachIndex([&](size_t i, size_t j)
            {
                if (i > 0)
                {
                    system->A(i, j).center += 1.0;
                }
                if (i < system->A.Width() - 1)
                {
                    system->A(i, j).center += 1.0;
                    system->A(i, j).right -= 1.0;
                }

                if (j > 0)
                {
                    system->A(i, j).center += 1.0;
                }
                else
                {
                    system->b(i, j) += 1.0;
                }

                if (j < system->A.Height() - 1)
                {
                    system->A(i, j).center += 1.0;
                    system->A(i, j).up -= 1.0;
                }
                else
                {
                    system->b(i, j) -= 1.0;
                }
            });
        }

        static void BuildTestCompressedLinearSystem(FDMCompressedLinearSystem2* system, const Size2& size)
        {
            Array2<size_t> coordToIndex(size);
            const auto acc = coordToIndex.ConstAccessor();

            coordToIndex.ForEachIndex([&](size_t i, size_t j)
            {
                const size_t cIdx = acc.Index(i, j);

                coordToIndex[cIdx] = system->b.size();
                double bij = 0.0;

                std::vector<double> row(1, 0.0);
                std::vector<size_t> colIdx(1, cIdx);

                if (i > 0)
                {
                    const size_t lIdx = acc.Index(i - 1, j);
                    row[0] += 1.0;
                    row.push_back(-1.0);
                    colIdx.push_back(lIdx);
                }
                if (i < size.x - 1)
                {
                    const size_t rIdx = acc.Index(i + 1, j);
                    row[0] += 1.0;
                    row.push_back(-1.0);
                    colIdx.push_back(rIdx);
                }

                if (j > 0)
                {
                    const size_t dIdx = acc.Index(i, j - 1);
                    row[0] += 1.0;
                    row.push_back(-1.0);
                    colIdx.push_back(dIdx);
                }
                else
                {
                    bij += 1.0;
                }

                if (j < size.y - 1)
                {
                    const size_t uIdx = acc.Index(i, j + 1);
                    row[0] += 1.0;
                    row.push_back(-1.0);
                    colIdx.push_back(uIdx);
                }
                else
                {
                    bij -= 1.0;
                }

                system->A.AddRow(row, colIdx);
                system->b.Append(bij);
            });

            system->x.Resize(system->b.size(), 0.0);
        }
    };
}

#endif