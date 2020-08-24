#include "UnitTestsUtils.hpp"
#include "pch.hpp"

#include <Core/Geometry/BVH2.hpp>

using namespace CubbyFlow;

TEST(BVH2, Constructors)
{
    BVH2<Vector2D> bvh;
    EXPECT_EQ(bvh.begin(), bvh.end());
}

TEST(BVH2, BasicGetters)
{
    BVH2<Vector2D> bvh;

    std::vector<Vector2D> points{ Vector2D{ 0, 0 }, Vector2D{ 1, 1 } };
    std::vector<BoundingBox2D> bounds{ points.size() };
    size_t i = 0;
    BoundingBox2D rootBounds;

    std::generate(bounds.begin(), bounds.end(), [&]() {
        const auto c = points[i++];
        BoundingBox2D box{ c, c };

        box.Expand(0.1);
        rootBounds.Merge(box);

        return box;
    });

    bvh.Build(points, bounds);

    EXPECT_EQ(2u, bvh.GetNumberOfItems());
    EXPECT_VECTOR2_EQ(points[0], bvh.GetItem(0));
    EXPECT_VECTOR2_EQ(points[1], bvh.GetItem(1));
    EXPECT_EQ(3u, bvh.GetNumberOfNodes());
    EXPECT_EQ(1u, bvh.GetChildren(0).first);
    EXPECT_EQ(2u, bvh.GetChildren(0).second);
    EXPECT_FALSE(bvh.IsLeaf(0));
    EXPECT_TRUE(bvh.IsLeaf(1));
    EXPECT_TRUE(bvh.IsLeaf(2));
    EXPECT_BOUNDING_BOX2_EQ(rootBounds, bvh.GetNodeBound(0));
    EXPECT_BOUNDING_BOX2_EQ(bounds[0], bvh.GetNodeBound(1));
    EXPECT_BOUNDING_BOX2_EQ(bounds[1], bvh.GetNodeBound(2));
    EXPECT_EQ(bvh.end(), bvh.GetItemOfNode(0));
    EXPECT_EQ(bvh.begin(), bvh.GetItemOfNode(1));
    EXPECT_EQ(bvh.begin() + 1, bvh.GetItemOfNode(2));
}

TEST(BVH2, Nearest)
{
    BVH2<Vector2D> bvh;

    auto distanceFunc = [](const Vector2D& a, const Vector2D& b) {
        return a.DistanceTo(b);
    };

    size_t numSamples = GetNumberOfSamplePoints2();
    std::vector<Vector2D> points(GetSamplePoints2(),
                                 GetSamplePoints2() + numSamples);

    std::vector<BoundingBox2D> bounds(points.size());
    size_t i = 0;

    std::generate(bounds.begin(), bounds.end(), [&]() {
        auto c = points[i++];
        BoundingBox2D box(c, c);

        box.Expand(0.1);

        return box;
    });

    bvh.Build(points, bounds);

    Vector2D testPt(0.5, 0.5);
    auto nearest = bvh.GetNearestNeighbor(testPt, distanceFunc);
    ptrdiff_t answerIdx = 0;
    double bestDist = testPt.DistanceTo(points[answerIdx]);

    for (i = 1; i < numSamples; ++i)
    {
        double dist = testPt.DistanceTo(GetSamplePoints2()[i]);
        if (dist < bestDist)
        {
            bestDist = dist;
            answerIdx = i;
        }
    }

    EXPECT_EQ(answerIdx, nearest.item - &bvh.GetItem(0));
}

TEST(BVH2, BBoxIntersects)
{
    BVH2<Vector2D> bvh;

    auto overlapsFunc = [](const Vector2D& pt, const BoundingBox2D& bbox) {
        BoundingBox2D box(pt, pt);

        box.Expand(0.1);

        return bbox.Overlaps(box);
    };

    size_t numSamples = GetNumberOfSamplePoints2();
    std::vector<Vector2D> points(GetSamplePoints2(),
                                 GetSamplePoints2() + numSamples);

    std::vector<BoundingBox2D> bounds(points.size());
    size_t i = 0;

    std::generate(bounds.begin(), bounds.end(), [&]() {
        auto c = points[i++];
        BoundingBox2D box(c, c);

        box.Expand(0.1);

        return box;
    });

    bvh.Build(points, bounds);

    BoundingBox2D testBox({ 0.25, 0.15 }, { 0.5, 0.6 });
    bool hasOverlaps = false;

    for (i = 0; i < numSamples; ++i)
    {
        hasOverlaps |= overlapsFunc(GetSamplePoints2()[i], testBox);
    }

    EXPECT_EQ(hasOverlaps, bvh.IsIntersects(testBox, overlapsFunc));

    BoundingBox2D testBox2({ 0.2, 0.2 }, { 0.6, 0.5 });
    hasOverlaps = false;

    for (i = 0; i < numSamples; ++i)
    {
        hasOverlaps |= overlapsFunc(GetSamplePoints2()[i], testBox2);
    }

    EXPECT_EQ(hasOverlaps, bvh.IsIntersects(testBox2, overlapsFunc));
}

TEST(BVH2, RayIntersects)
{
    BVH2<BoundingBox2D> bvh;

    auto intersectsFunc = [](const BoundingBox2D& a, const Ray2D& ray) {
        return a.Intersects(ray);
    };

    size_t numSamples = GetNumberOfSamplePoints2();
    std::vector<BoundingBox2D> items(numSamples / 2);
    size_t i = 0;

    std::generate(items.begin(), items.end(), [&]() {
        auto c = GetSamplePoints2()[i++];
        BoundingBox2D box(c, c);

        box.Expand(0.1);

        return box;
    });

    bvh.Build(items, items);

    for (i = 0; i < numSamples / 2; ++i)
    {
        Ray2D ray(GetSampleDirs2()[i + numSamples / 2],
                  GetSampleDirs2()[i + numSamples / 2]);

        // ad-hoc search
        bool ansInts = false;
        for (size_t j = 0; j < numSamples / 2; ++j)
        {
            if (intersectsFunc(items[j], ray))
            {
                ansInts = true;
                break;
            }
        }

        // bvh search
        bool octInts = bvh.IsIntersects(ray, intersectsFunc);

        EXPECT_EQ(ansInts, octInts);
    }
}

TEST(BVH2, ClosestIntersection)
{
    BVH2<BoundingBox2D> bvh;

    auto intersectsFunc = [](const BoundingBox2D& a, const Ray2D& ray) {
        auto bboxResult = a.ClosestIntersection(ray);

        if (bboxResult.isIntersecting)
        {
            return bboxResult.near;
        }
        else
        {
            return std::numeric_limits<double>::max();
        }
    };

    size_t numSamples = GetNumberOfSamplePoints2();
    std::vector<BoundingBox2D> items(numSamples / 2);
    size_t i = 0;

    std::generate(items.begin(), items.end(), [&]() {
        auto c = GetSamplePoints2()[i++];
        BoundingBox2D box(c, c);

        box.Expand(0.1);

        return box;
    });

    bvh.Build(items, items);

    for (i = 0; i < numSamples / 2; ++i)
    {
        Ray2D ray(GetSamplePoints2()[i + numSamples / 2],
                  GetSampleDirs2()[i + numSamples / 2]);

        // ad-hoc search
        ClosestIntersectionQueryResult2<BoundingBox2D> ansInts;
        for (size_t j = 0; j < numSamples / 2; ++j)
        {
            double dist = intersectsFunc(items[j], ray);
            if (dist < ansInts.distance)
            {
                ansInts.distance = dist;
                ansInts.item = &bvh.GetItem(j);
            }
        }

        // bvh search
        auto bvhInts = bvh.GetClosestIntersection(ray, intersectsFunc);

        EXPECT_DOUBLE_EQ(ansInts.distance, bvhInts.distance);
        EXPECT_EQ(ansInts.item, bvhInts.item);
    }
}

TEST(BVH2, ForEachOverlappingItems)
{
    BVH2<Vector2D> bvh;

    auto overlapsFunc = [](const Vector2D& pt, const BoundingBox2D& bbox) {
        return bbox.Contains(pt);
    };

    size_t numSamples = GetNumberOfSamplePoints2();
    std::vector<Vector2D> points(GetSamplePoints2(),
                                 GetSamplePoints2() + numSamples);

    std::vector<BoundingBox2D> bounds(points.size());
    size_t i = 0;

    std::generate(bounds.begin(), bounds.end(), [&]() {
        auto c = points[i++];
        BoundingBox2D box(c, c);

        box.Expand(0.1);

        return box;
    });

    bvh.Build(points, bounds);

    BoundingBox2D testBox({ 0.2, 0.2 }, { 0.6, 0.5 });
    size_t numOverlaps = 0;

    for (i = 0; i < numSamples; ++i)
    {
        numOverlaps += overlapsFunc(GetSamplePoints2()[i], testBox);
    }

    size_t measured = 0;
    bvh.ForEachIntersectingItem(testBox, overlapsFunc, [&](const Vector2D& pt) {
        EXPECT_TRUE(overlapsFunc(pt, testBox));
        ++measured;
    });

    EXPECT_EQ(numOverlaps, measured);
}