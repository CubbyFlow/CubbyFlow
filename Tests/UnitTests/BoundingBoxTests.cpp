#include "gtest/gtest.h"

#include <Core/Geometry/BoundingBox.hpp>

using namespace CubbyFlow;

TEST(BoundingBox, Constructors)
{
    {
        BoundingBox<double, 2> box;

        EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(),
                         box.lowerCorner[0]);
        EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(),
                         box.lowerCorner[1]);

        EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(),
                         box.upperCorner[0]);
        EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(),
                         box.upperCorner[1]);
    }

    {
        BoundingBox<double, 2> box({ -2.0, 3.0 }, { 4.0, -2.0 });

        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner[0]);
        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner[1]);

        EXPECT_DOUBLE_EQ(4.0, box.upperCorner[0]);
        EXPECT_DOUBLE_EQ(3.0, box.upperCorner[1]);
    }

    {
        BoundingBox<double, 2> box({ -2.0, 3.0 }, { 4.0, -2.0 });
        BoundingBox<double, 2> box2(box);

        EXPECT_DOUBLE_EQ(-2.0, box2.lowerCorner[0]);
        EXPECT_DOUBLE_EQ(-2.0, box2.lowerCorner[1]);

        EXPECT_DOUBLE_EQ(4.0, box2.upperCorner[0]);
        EXPECT_DOUBLE_EQ(3.0, box2.upperCorner[1]);
    }
}

TEST(BoundingBox, Overlaps)
{
    // x-axis is not overlapping
    {
        BoundingBox<double, 2> box1({ -2.0, -2.0 }, { 4.0, 3.0 });
        BoundingBox<double, 2> box2({ 5.0, 1.0 }, { 8.0, 2.0 });

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // y-axis is not overlapping
    {
        BoundingBox<double, 2> box1({ -2.0, -2.0 }, { 4.0, 3.0 });
        BoundingBox<double, 2> box2({ 3.0, 4.0 }, { 8.0, 6.0 });

        EXPECT_FALSE(box1.Overlaps(box2));
    }

    // overlapping
    {
        BoundingBox<double, 2> box1({ -2.0, -2.0 }, { 4.0, 3.0 });
        BoundingBox<double, 2> box2({ 3.0, 1.0 }, { 8.0, 2.0 });

        EXPECT_TRUE(box1.Overlaps(box2));
    }
}

TEST(BoundingBox, Contains)
{
    // Not containing (x-axis is out)
    {
        BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
        Vector<double, 2> point({ -3.0, 0.0 });

        EXPECT_FALSE(box.Contains(point));
    }

    // Not containing (y-axis is out)
    {
        BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
        Vector<double, 2> point({ 2.0, 3.5 });

        EXPECT_FALSE(box.Contains(point));
    }

    // Containing
    {
        BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
        Vector<double, 2> point({ 2.0, 0.0 });

        EXPECT_TRUE(box.Contains(point));
    }
}

TEST(BoundingBox, MidPoint)
{
    BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
    Vector<double, 2> midPoint = box.MidPoint();

    EXPECT_DOUBLE_EQ(1.0, midPoint[0]);
    EXPECT_DOUBLE_EQ(0.5, midPoint[1]);
}

TEST(BoundingBox, DiagonalLength)
{
    BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
    double diagLen = box.DiagonalLength();

    EXPECT_DOUBLE_EQ(std::sqrt(6.0 * 6.0 + 5.0 * 5.0), diagLen);
}

TEST(BoundingBox, DiagonalLengthSquared)
{
    BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
    double diagLenSqr = box.DiagonalLengthSquared();

    EXPECT_DOUBLE_EQ(6.0 * 6.0 + 5.0 * 5.0, diagLenSqr);
}

TEST(BoundingBox, Reset)
{
    BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
    box.Reset();

    static const double maxDouble = std::numeric_limits<double>::max();

    EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner[0]);
    EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner[1]);

    EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner[0]);
    EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner[1]);
}

TEST(BoundingBox, Merge)
{
    // Merge with point
    {
        BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
        Vector<double, 2> point({ 5.0, 1.0 });

        box.Merge(point);

        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner[0]);
        EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner[1]);

        EXPECT_DOUBLE_EQ(5.0, box.upperCorner[0]);
        EXPECT_DOUBLE_EQ(3.0, box.upperCorner[1]);
    }

    // Merge with other box
    {
        BoundingBox<double, 2> box1({ -2.0, -2.0 }, { 4.0, 3.0 });
        BoundingBox<double, 2> box2({ 3.0, 1.0 }, { 8.0, 2.0 });

        box1.Merge(box2);

        EXPECT_DOUBLE_EQ(-2.0, box1.lowerCorner[0]);
        EXPECT_DOUBLE_EQ(-2.0, box1.lowerCorner[1]);

        EXPECT_DOUBLE_EQ(8.0, box1.upperCorner[0]);
        EXPECT_DOUBLE_EQ(3.0, box1.upperCorner[1]);
    }
}

TEST(BoundingBox, Expand)
{
    BoundingBox<double, 2> box({ -2.0, -2.0 }, { 4.0, 3.0 });
    box.Expand(3.0);

    EXPECT_DOUBLE_EQ(-5.0, box.lowerCorner[0]);
    EXPECT_DOUBLE_EQ(-5.0, box.lowerCorner[1]);

    EXPECT_DOUBLE_EQ(7.0, box.upperCorner[0]);
    EXPECT_DOUBLE_EQ(6.0, box.upperCorner[1]);
}