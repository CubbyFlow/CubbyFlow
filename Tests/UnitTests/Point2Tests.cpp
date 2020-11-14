#include "pch.hpp"

#include <Core/Geometry/Point2.hpp>

using namespace CubbyFlow;

TEST(Point2, Constructors)
{
    Point2F pt;
    EXPECT_FLOAT_EQ(0.f, pt.x);
    EXPECT_FLOAT_EQ(0.f, pt.y);

    Point2F pt2(5.f, 3.f);
    EXPECT_FLOAT_EQ(5.f, pt2.x);
    EXPECT_FLOAT_EQ(3.f, pt2.y);

    Point2F pt5 = { 7.f, 6.f };
    EXPECT_FLOAT_EQ(7.f, pt5.x);
    EXPECT_FLOAT_EQ(6.f, pt5.y);

    Point2F pt6(pt5);
    EXPECT_FLOAT_EQ(7.f, pt6.x);
    EXPECT_FLOAT_EQ(6.f, pt6.y);
}

TEST(Point2, SetMethods)
{
    Point2F pt;
    pt.Set(4.f, 2.f);
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(2.f, pt.y);

    auto list = { 0.f, 5.f };
    pt.Set(list);
    EXPECT_FLOAT_EQ(0.f, pt.x);
    EXPECT_FLOAT_EQ(5.f, pt.y);

    pt.Set(Point2F(9.f, 8.f));
    EXPECT_FLOAT_EQ(9.f, pt.x);
    EXPECT_FLOAT_EQ(8.f, pt.y);
}

TEST(Point2, BasicSetterMethods)
{
    Point2F pt(3.f, 9.f);
    pt.SetZero();
    EXPECT_FLOAT_EQ(0.f, pt.x);
    EXPECT_FLOAT_EQ(0.f, pt.y);
}

TEST(Point2, BinaryOperatorMethods)
{
    Point2F pt(3.f, 9.f);
    pt = pt.Add(4.f);
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(13.f, pt.y);

    pt = pt.Add(Point2F(-2.f, 1.f));
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(14.f, pt.y);

    pt = pt.Sub(8.f);
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt.Sub(Point2F(-5.f, 3.f));
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt = pt.Mul(2.f);
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt.Mul(Point2F(3.f, -2.f));
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt = pt.Div(4.f);
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt = pt.Div(Point2F(3.f, -1.f));
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

TEST(Point2, BinaryInverseOperatorMethods)
{
    Point2F pt(3.f, 9.f);
    pt = pt.RSub(8.f);
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(-1.f, pt.y);

    pt = pt.RSub(Point2F(-5.f, 3.f));
    EXPECT_FLOAT_EQ(-10.f, pt.x);
    EXPECT_FLOAT_EQ(4.f, pt.y);

    pt = Point2F(-4.f, -3.f);
    pt = pt.RDiv(12.f);
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, -4.f);

    pt = pt.RDiv(Point2F(3.f, -16.f));
    EXPECT_FLOAT_EQ(-1.f, pt.x);
    EXPECT_FLOAT_EQ(4.f, pt.y);
}

TEST(Point2, AugmentedOperatorMethods)
{
    Point2F pt(3.f, 9.f);
    pt.IAdd(4.f);
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 13.f);

    pt.IAdd(Point2F(-2.f, 1.f));
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 14.f);

    pt.ISub(8.f);
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt.ISub(Point2F(-5.f, 3.f));
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt.IMul(2.f);
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt.IMul(Point2F(3.f, -2.f));
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt.IDiv(4.f);
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt.IDiv(Point2F(3.f, -1.f));
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

TEST(Point2, AtMethod)
{
    Point2F pt(8.f, 9.f);
    EXPECT_FLOAT_EQ(pt.At(0), 8.f);
    EXPECT_FLOAT_EQ(pt.At(1), 9.f);

    pt.At(0) = 7.f;
    pt.At(1) = 6.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);
}

TEST(Point2, BasicGetterMethods)
{
    Point2F pt(3.f, 7.f), pt2(-3.f, -7.f);

    float sum = pt.Sum();
    EXPECT_FLOAT_EQ(sum, 10.f);

    float min = pt.Min();
    EXPECT_FLOAT_EQ(min, 3.f);

    float max = pt.Max();
    EXPECT_FLOAT_EQ(max, 7.f);

    float absmin = pt2.AbsMin();
    EXPECT_FLOAT_EQ(absmin, -3.f);

    float absmax = pt2.AbsMax();
    EXPECT_FLOAT_EQ(absmax, -7.f);

    size_t daxis = pt.DominantAxis();
    EXPECT_EQ(daxis, static_cast<size_t>(1));

    size_t saxis = pt.SubdominantAxis();
    EXPECT_EQ(saxis, static_cast<size_t>(0));
}

TEST(Point2, BracketOperator)
{
    Point2F pt(8.f, 9.f);
    EXPECT_FLOAT_EQ(pt[0], 8.f);
    EXPECT_FLOAT_EQ(pt[1], 9.f);

    pt[0] = 7.f;
    pt[1] = 6.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);
}

TEST(Point2, AssignmentOperator)
{
    Point2F pt(5.f, 1.f);
    Point2F pt2;
    pt2 = pt;
    EXPECT_FLOAT_EQ(5.f, pt2.x);
    EXPECT_FLOAT_EQ(pt2.y, 1.f);
}

TEST(Point2, AugmentedOperators)
{
    Point2F pt(3.f, 9.f);
    pt += 4.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 13.f);

    pt += Point2F(-2.f, 1.f);
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 14.f);

    pt -= 8.f;
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt -= Point2F(-5.f, 3.f);
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt *= 2.f;
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt *= Point2F(3.f, -2.f);
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt /= 4.f;
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt /= Point2F(3.f, -1.f);
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}

TEST(Point2, EqualOperator)
{
    Point2F pt, pt2(3.f, 7.f), pt3(3.f, 5.f), pt4(5.f, 1.f);
    pt = pt2;
    EXPECT_TRUE(pt == pt2);
    EXPECT_FALSE(pt == pt3);
    EXPECT_FALSE(pt != pt2);
    EXPECT_TRUE(pt != pt3);
    EXPECT_TRUE(pt != pt4);
}

TEST(Point2, MinMaxFunction)
{
    Point2F pt(5.f, 1.f);
    Point2F pt2(3.f, 3.f);
    Point2F minPoint = Min(pt, pt2);
    Point2F maxPoint = Max(pt, pt2);
    EXPECT_EQ(Point2F(3.f, 1.f), minPoint);
    EXPECT_EQ(Point2F(5.f, 3.f), maxPoint);
}

TEST(Point2, ClampFunction)
{
    Point2F pt(2.f, 4.f), low(3.f, -1.f), high(5.f, 2.f);
    Point2F clampedVec = Clamp(pt, low, high);
    EXPECT_EQ(Point2F(3.f, 2.f), clampedVec);
}

TEST(Point2, CeilFloorFunction)
{
    Point2F pt(2.2f, 4.7f);
    Point2F ceilVec = Ceil(pt);
    EXPECT_EQ(Point2F(3.f, 5.f), ceilVec);

    Point2F floorVec = Floor(pt);
    EXPECT_EQ(Point2F(2.f, 4.f), floorVec);
}

TEST(Point2, BinaryOperators)
{
    Point2F pt(3.f, 9.f);
    pt = pt + 4.f;
    EXPECT_FLOAT_EQ(7.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 13.f);

    pt = pt + Point2F(-2.f, 1.f);
    EXPECT_FLOAT_EQ(5.f, pt.x);
    EXPECT_FLOAT_EQ(pt.y, 14.f);

    pt = pt - 8.f;
    EXPECT_FLOAT_EQ(-3.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt - Point2F(-5.f, 3.f);
    EXPECT_FLOAT_EQ(2.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);

    pt = pt * 2.f;
    EXPECT_FLOAT_EQ(4.f, pt.x);
    EXPECT_FLOAT_EQ(6.f, pt.y);

    pt = pt * Point2F(3.f, -2.f);
    EXPECT_FLOAT_EQ(12.f, pt.x);
    EXPECT_FLOAT_EQ(-12.f, pt.y);

    pt = pt / 4.f;
    EXPECT_FLOAT_EQ(3.f, pt.x);
    EXPECT_FLOAT_EQ(-3.f, pt.y);

    pt = pt / Point2F(3.f, -1.f);
    EXPECT_FLOAT_EQ(1.f, pt.x);
    EXPECT_FLOAT_EQ(3.f, pt.y);
}