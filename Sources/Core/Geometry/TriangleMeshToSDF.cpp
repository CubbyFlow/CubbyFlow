/*************************************************************************
> File Name: TriangleMeshToSDF.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Generates signed-distance field out of given triangle mesh.
> Created Time: 2017/08/24
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
// This code is ported from Christopher Batty's SDFGen software.
// (https://github.com/christopherbatty/SDFGen)
//
// The MIT License (MIT)
//
// Copyright (c) 2015, Christopher Batty
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <Core/Array/Array3.hpp>
#include <Core/Geometry/Triangle3.h>
#include <Core/Geometry/TriangleMesh3.h>
#include <Core/Geometry/TriangleMeshToSDF.h>
#include <Core/Grid/ScalarGrid3.h>
#include <Core/Point/Point3.h>
#include <Core/Size/Size3.h>
#include <Core/Utils/Constants.h>
#include <Core/Utils/Macros.h>
#include <Core/Vector/Vector3.h>

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
	static void CheckNeighbor(
		const TriangleMesh3& mesh,
		const Vector3D& gx,
		ssize_t i0,	ssize_t j0, ssize_t k0,
		ssize_t i1, ssize_t j1, ssize_t k1,
		ScalarGrid3* sdf,
		Array3<size_t>* closestTri)
	{
		if ((*closestTri)(i1, j1, k1) != std::numeric_limits<size_t>::max())
		{
			size_t t = (*closestTri)(i1, j1, k1);
			Triangle3 tri = mesh.Triangle(t);
			double d = tri.ClosestDistance(gx);

			if (d < (*sdf)(i0, j0, k0))
			{
				(*sdf)(i0, j0, k0) = d;
				(*closestTri)(i0, j0, k0) = (*closestTri)(i1, j1, k1);
			}
		}
	}

	static void Sweep(
		const TriangleMesh3& mesh,
		int di, int dj, int dk,
		ScalarGrid3* sdf,
		Array3<size_t>* closestTri)
	{
		Size3 size = sdf->GetDataSize();
		Vector3D h = sdf->GridSpacing();
		Vector3D origin = sdf->GetDataOrigin();

		ssize_t ni = static_cast<ssize_t>(size.x);
		ssize_t nj = static_cast<ssize_t>(size.y);
		ssize_t nk = static_cast<ssize_t>(size.z);

		ssize_t i0, i1;
		if (di > 0)
		{
			i0 = 1;
			i1 = ni;
		}
		else
		{
			i0 = ni - 2;
			i1 = -1;
		}

		ssize_t j0, j1;
		if (dj > 0)
		{
			j0 = 1;
			j1 = nj;
		}
		else
		{
			j0 = nj - 2;
			j1 = -1;
		}

		ssize_t k0, k1;
		if (dk > 0)
		{
			k0 = 1;
			k1 = nk;
		}
		else
		{
			k0 = nk - 2;
			k1 = -1;
		}

		for (ssize_t k = k0; k != k1; k += dk)
		{
			for (ssize_t j = j0; j != j1; j += dj)
			{
				for (ssize_t i = i0; i != i1; i += di)
				{
					Vector3D gx({ i, j, k });
					gx *= h;
					gx += origin;

					CheckNeighbor(mesh, gx, i, j, k, i - di, j, k, sdf, closestTri);
					CheckNeighbor(mesh, gx, i, j, k, i, j - dj, k, sdf, closestTri);
					CheckNeighbor(mesh, gx, i, j, k, i - di, j - dj, k, sdf, closestTri);
					CheckNeighbor(mesh, gx, i, j, k, i, j, k - dk, sdf, closestTri);
					CheckNeighbor(mesh, gx, i, j, k, i - di, j, k - dk, sdf, closestTri);
					CheckNeighbor(mesh, gx, i, j, k, i, j - dj, k - dk, sdf, closestTri);
					CheckNeighbor(mesh, gx, i, j, k, i - di, j - dj, k - dk, sdf, closestTri);
				}
			}
		}
	}

	// calculate twice signed area of triangle (0,0)-(x1,y1)-(x2,y2)
	// return an SOS-determined sign (-1, +1, or 0 only if it's a truly degenerate triangle)
	static int Orientation(
		double x1, double y1,
		double x2, double y2,
		double* twiceSignedArea)
	{
		(*twiceSignedArea) = y1 * x2 - x1 * y2;

		if ((*twiceSignedArea) > 0)
		{
			return 1;
		}
		if ((*twiceSignedArea) < 0)
		{
			return -1;
		}
		if (y2 > y1)
		{
			return 1;
		}
		if (y2 < y1)
		{
			return -1;
		}
		if (x1 > x2)
		{
			return 1;
		}
		if (x1 < x2)
		{
			return -1;
		}

		// only true when x1==x2 and y1==y2
		return 0;
	}

	// robust test of (x0,y0) in the triangle (x1,y1)-(x2,y2)-(x3,y3)
	// if true is returned, the barycentric coordinates are set in a,b,c.
	static bool PointInTriangle2D(
		double x0, double y0,
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		double* a, double* b, double* c)
	{
		x1 -= x0;
		x2 -= x0;
		x3 -= x0;
		y1 -= y0;
		y2 -= y0;
		y3 -= y0;

		int signa = Orientation(x2, y2, x3, y3, a);
		if (signa == 0)
		{
			return false;
		}

		int signb = Orientation(x3, y3, x1, y1, b);
		if (signb != signa)
		{
			return false;
		}

		int signc = Orientation(x1, y1, x2, y2, c);
		if (signc != signa)
		{
			return false;
		}

		double sum = (*a) + (*b) + (*c);

		// if the SOS signs match and are nonzero, there's no way all of a, b, and c are zero.
		assert(sum != 0);

		*a /= sum;
		*b /= sum;
		*c /= sum;

		return true;
	}

	void TriangleMeshToSDF(const TriangleMesh3& mesh, ScalarGrid3* sdf, const unsigned int exactBand)
	{
		Size3 size = sdf->GetDataSize();
		if (size.x * size.y * size.z == 0)
		{
			return;
		}

		// Upper bound on distance
		sdf->Fill(sdf->BoundingBox().DiagonalLength());
		Vector3D h = sdf->GridSpacing();
		Vector3D origin = sdf->GetDataOrigin();

		Array3<size_t> closestTri(size, std::numeric_limits<size_t>::max());

		// Intersection_count(i,j,k) is # of tri intersections in (i-1,i]x{j}x{k}
		Array3<unsigned int> intersectionCount(size, 0);

		// We begin by initializing distances near the mesh, and figuring out
		// intersection counts
		auto gridPos = sdf->GetDataPosition();

		size_t nTri = mesh.NumberOfTriangles();
		ssize_t bandwidth = static_cast<ssize_t>(exactBand);
		ssize_t maxSizeX = static_cast<ssize_t>(size.x);
		ssize_t maxSizeY = static_cast<ssize_t>(size.y);
		ssize_t maxSizeZ = static_cast<ssize_t>(size.z);

		for (size_t t = 0; t < nTri; ++t)
		{
			Point3UI indices = mesh.PointIndex(t);

			Triangle3 tri = mesh.Triangle(t);

			Vector3D pt1 = mesh.Point(indices.x);
			Vector3D pt2 = mesh.Point(indices.y);
			Vector3D pt3 = mesh.Point(indices.z);

			// Normalize coordinates
			Vector3D f1 = (pt1 - origin) / h;
			Vector3D f2 = (pt2 - origin) / h;
			Vector3D f3 = (pt3 - origin) / h;

			// Do distances nearby
			ssize_t i0 = static_cast<ssize_t>(std::min({ f1.x, f2.x, f3.x }));
			i0 = std::clamp(i0 - bandwidth, ZERO_SSIZE, maxSizeX - 1);
			ssize_t i1 = static_cast<ssize_t>(std::max({ f1.x, f2.x, f3.x }));
			i1 = std::clamp(i1 + bandwidth + 1, ZERO_SSIZE, maxSizeX - 1);

			ssize_t j0 = static_cast<ssize_t>(std::min({ f1.y, f2.y, f3.y }));
			j0 = std::clamp(j0 - bandwidth, ZERO_SSIZE, maxSizeY - 1);
			ssize_t j1 = static_cast<ssize_t>(std::max({ f1.y, f2.y, f3.y }));
			j1 = std::clamp(j1 + bandwidth + 1, ZERO_SSIZE, maxSizeY - 1);

			ssize_t k0 = static_cast<ssize_t>(std::min({ f1.z, f2.z, f3.z }));
			k0 = std::clamp(k0 - bandwidth, ZERO_SSIZE, maxSizeZ - 1);
			ssize_t k1 = static_cast<ssize_t>(std::max({ f1.z, f2.z, f3.z }));
			k1 = std::clamp(k1 + bandwidth + 1, ZERO_SSIZE, maxSizeZ - 1);

			for (ssize_t k = k0; k <= k1; ++k)
			{
				for (ssize_t j = j0; j <= j1; ++j)
				{
					for (ssize_t i = i0; i <= i1; ++i)
					{
						Vector3D gx = gridPos(i, j, k);
						double d = tri.ClosestDistance(gx);

						if (d < (*sdf)(i, j, k))
						{
							(*sdf)(i, j, k) = d;
							closestTri(i, j, k) = t;
						}
					}
				}
			}

			// Do intersection counts
			j0 = static_cast<ssize_t>(std::ceil(std::min({ f1.y, f2.y, f3.y })));
			j0 = std::clamp(j0 - bandwidth, ZERO_SSIZE, maxSizeY - 1);
			j1 = static_cast<ssize_t>(std::floor(std::max({ f1.y, f2.y, f3.y })));
			j1 = std::clamp(j1 + bandwidth + 1, ZERO_SSIZE, maxSizeY - 1);
			k0 = static_cast<ssize_t>(std::ceil(std::min({ f1.z, f2.z, f3.z })));
			k0 = std::clamp(k0 - bandwidth, ZERO_SSIZE, maxSizeZ - 1);
			k1 = static_cast<ssize_t>(std::floor(std::max({ f1.z, f2.z, f3.z })));
			k1 = std::clamp(k1 + bandwidth + 1, ZERO_SSIZE, maxSizeZ - 1);

			for (ssize_t k = k0; k <= k1; ++k)
			{
				for (ssize_t j = j0; j <= j1; ++j)
				{
					double a, b, c;
					double jD = static_cast<double>(j);
					double kD = static_cast<double>(k);

					if (PointInTriangle2D(jD, kD, f1.y, f1.z, f2.y, f2.z, f3.y, f3.z, &a, &b, &c))
					{
						// intersection i coordinate
						double fi = a * f1.x + b * f2.x + c * f3.x;

						// intersection is in (iInterval - 1, iInterval]
						int iInterval = static_cast<int>(std::ceil(fi));
						if (iInterval < 0)
						{
							// we enlarge the first interval to include everything
							// to the -x direction
							++intersectionCount(0, j, k);
						}
						else if (iInterval < static_cast<int>(size.x))
						{
							++intersectionCount(iInterval, j, k);
						}

						// we ignore intersections that are beyond the +x side of the grid
					}
				}
			}
		}

		// and now we fill in the rest of the distances with fast sweeping
		for (unsigned int pass = 0; pass < 2; ++pass)
		{
			Sweep(mesh, +1, +1, +1, sdf, &closestTri);
			Sweep(mesh, -1, -1, -1, sdf, &closestTri);
			Sweep(mesh, +1, +1, -1, sdf, &closestTri);
			Sweep(mesh, -1, -1, +1, sdf, &closestTri);
			Sweep(mesh, +1, -1, +1, sdf, &closestTri);
			Sweep(mesh, -1, +1, -1, sdf, &closestTri);
			Sweep(mesh, +1, -1, -1, sdf, &closestTri);
			Sweep(mesh, -1, +1, +1, sdf, &closestTri);
		}

		// then figure out signs (inside/outside) from intersection counts
		for (size_t k = 0; k < size.z; ++k)
		{
			for (size_t j = 0; j < size.y; ++j)
			{
				unsigned int totalCount = 0U;

				for (size_t i = 0; i < size.x; ++i)
				{
					totalCount += intersectionCount(i, j, k);

					// if parity of intersections so far is odd,
					if (totalCount % 2 == 1)
					{
						// we are inside the mesh
						(*sdf)(i, j, k) = -(*sdf)(i, j, k);
					}
				}
			}
		}
	}
}
