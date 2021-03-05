#include "UnitTestsUtils.hpp"
#include "pch.hpp"

#include <Core/Geometry/BoundingBox.hpp>

using namespace CubbyFlow;

TEST(BoundingBox2, Constructors)
{
    {
        BoundingBox2D box;

        EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), box.lowerCorner.x);
        EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), box.lowerCorner.y);

        EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(),
                         box.upperCorner.x);
        EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(),
                         box.upperCorner.y);
    }

    {
        BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));

        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.y);

        EXPECT_DOUBLE_EQ(4.0, box.upperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box.upperCorner.y);
    }

    {
        BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));
        BoundingBox2D box2(box);

        EXPECT_DOUBLE_EQ(-2.0, box2.lowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box2.lowerCorner.y);

        EXPECT_DOUBLE_EQ(4.0, box2.upperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box2.upperCorner.y);
    }
}

TEST(BoundingBox2, BasicGetters)
{
    BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));

    EXPECT_DOUBLE_EQ(6.0, box.Width());
    EXPECT_DOUBLE_EQ(5.0, box.Height());
    EXPECT_DOUBLE_EQ(6.0, box.Length(0));
    EXPECT_DOUBLE_EQ(5.0, box.Length(1));
}

TEST(BoundingBox2, Overlaps)
{
    // x-axis is not overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(5.0, 1.0), Vector2D(8.0, 2.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // y-axis is not overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 4.0), Vector2D(8.0, 6.0));

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 1.0), Vector2D(8.0, 2.0));

        EXPECT_TRUE(box1.Overlaps(box2));
    }
}

TEST(BoundingBox2, Contains)
{
    // Not containing (x-axis is out)
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(-3.0, 0.0);

        EXPECT_FALSE(box.Contains(point));
    }

    // Not containing (y-axis is out)
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(2.0, 3.5);

        EXPECT_FALSE(box.Contains(point));
    }

    // Containing
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(2.0, 0.0);

        EXPECT_TRUE(box.Contains(point));
    }
}

TEST(BoundingBox2, Intersects)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));

    Ray2D ray1(Vector2D(-3, 0), Vector2D(2, 1).Normalized());
    EXPECT_TRUE(box.Intersects(ray1));

    Ray2D ray2(Vector2D(3, -1), Vector2D(-1, 2).Normalized());
    EXPECT_TRUE(box.Intersects(ray2));

    Ray2D ray3(Vector2D(1, -5), Vector2D(2, 1).Normalized());
    EXPECT_FALSE(box.Intersects(ray3));
}

TEST(BoundingBox2, ClosestIntersection)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(1.0, 0.0));

    Ray2D ray1(Vector2D(-4, -3), Vector2D(1, 1).Normalized());
    BoundingBoxRayIntersectionD intersection1 = box.ClosestIntersection(ray1);
    EXPECT_TRUE(intersection1.isIntersecting);
    EXPECT_DOUBLE_EQ(Vector2D(2, 2).Length(), intersection1.near);
    EXPECT_DOUBLE_EQ(Vector2D(3, 3).Length(), intersection1.far);

    Ray2D ray2(Vector2D(0, -1), Vector2D(-2, 1).Normalized());
    BoundingBoxRayIntersectionD intersection2 = box.ClosestIntersection(ray2);
    EXPECT_TRUE(intersection2.isIntersecting);
    EXPECT_DOUBLE_EQ(Vector2D(2, 1).Length(), intersection2.near);
}

TEST(BoundingBox2, MidPoint)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    Vector2D midPoint = box.MidPoint();

    EXPECT_DOUBLE_EQ(1.0, midPoint.x);
    EXPECT_DOUBLE_EQ(0.5, midPoint.y);
}

TEST(BoundingBox2, DiagonalLength)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    double diagLen = box.DiagonalLength();

    EXPECT_DOUBLE_EQ(std::sqrt(6.0 * 6.0 + 5.0 * 5.0), diagLen);
}

TEST(BoundingBox2, DiagonalLengthSquared)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    double diagLenSqr = box.DiagonalLengthSquared();

    EXPECT_DOUBLE_EQ(6.0 * 6.0 + 5.0 * 5.0, diagLenSqr);
}

TEST(BoundingBox2, Reset)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    box.Reset();

    static const double maxDouble = std::numeric_limits<double>::max();

    EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner.x);
    EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner.y);

    EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner.x);
    EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner.y);
}

TEST(BoundingBox2, Merge)
{
    // Merge with point
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(5.0, 1.0);

        box.Merge(point);

        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.y);

        EXPECT_DOUBLE_EQ(5.0, box.upperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box.upperCorner.y);
    }

    // Merge with other box
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 1.0), Vector2D(8.0, 2.0));

        box1.Merge(box2);

        EXPECT_DOUBLE_EQ(-2.0, box1.lowerCorner.x);
        EXPECT_DOUBLE_EQ(-2.0, box1.lowerCorner.y);

        EXPECT_DOUBLE_EQ(8.0, box1.upperCorner.x);
        EXPECT_DOUBLE_EQ(3.0, box1.upperCorner.y);
    }
}

TEST(BoundingBox2, Expand)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    box.Expand(3.0);

    EXPECT_DOUBLE_EQ(-5.0, box.lowerCorner.x);
    EXPECT_DOUBLE_EQ(-5.0, box.lowerCorner.y);

    EXPECT_DOUBLE_EQ(7.0, box.upperCorner.x);
    EXPECT_DOUBLE_EQ(6.0, box.upperCorner.y);
}

TEST(BoundingBox2, Corner)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    EXPECT_VECTOR2_EQ(Vector2D(-2.0, -2.0), box.Corner(0));
    EXPECT_VECTOR2_EQ(Vector2D(4.0, -2.0), box.Corner(1));
    EXPECT_VECTOR2_EQ(Vector2D(-2.0, 3.0), box.Corner(2));
    EXPECT_VECTOR2_EQ(Vector2D(4.0, 3.0), box.Corner(3));
}

TEST(BoundingBox2, IsEmpty)
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    EXPECT_FALSE(box.IsEmpty());

    box.lowerCorner = Vector2D(5.0, 1.0);
    EXPECT_TRUE(box.IsEmpty());

    box.lowerCorner = Vector2D(2.0, 4.0);
    EXPECT_TRUE(box.IsEmpty());

    box.lowerCorner = Vector2D(4.0, 1.0);
    EXPECT_TRUE(box.IsEmpty());
}