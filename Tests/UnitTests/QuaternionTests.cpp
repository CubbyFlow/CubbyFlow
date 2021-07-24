#include "gtest/gtest.h"

#include <Core/Math/Quaternion.hpp>

using namespace CubbyFlow;

TEST(Quaternion, Constructors)
{
    {
        QuaternionD q;

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(0.0, q.x);
        EXPECT_DOUBLE_EQ(0.0, q.y);
        EXPECT_DOUBLE_EQ(0.0, q.z);
    }
    {
        QuaternionD q(1, 2, 3, 4);

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(2.0, q.x);
        EXPECT_DOUBLE_EQ(3.0, q.y);
        EXPECT_DOUBLE_EQ(4.0, q.z);
    }
    {
        QuaternionD q(QuaternionD(1, 2, 3, 4));

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(2.0, q.x);
        EXPECT_DOUBLE_EQ(3.0, q.y);
        EXPECT_DOUBLE_EQ(4.0, q.z);
    }
    {
        QuaternionD q({ 1, 2, 3, 4 });

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(2.0, q.x);
        EXPECT_DOUBLE_EQ(3.0, q.y);
        EXPECT_DOUBLE_EQ(4.0, q.z);
    }
    {
        // set with axis & angle
        Vector3D originalAxis = Vector3D(1, 3, 2).Normalized();
        double originalAngle = 0.4;

        QuaternionD q(originalAxis, originalAngle);

        Vector3D axis = q.Axis();
        double angle = q.Angle();

        EXPECT_DOUBLE_EQ(originalAxis.x, axis.x);
        EXPECT_DOUBLE_EQ(originalAxis.y, axis.y);
        EXPECT_DOUBLE_EQ(originalAxis.z, axis.z);
        EXPECT_DOUBLE_EQ(originalAngle, angle);
    }

    {
        // set with from & to vectors (90 degrees)
        Vector3D from(1, 0, 0);
        Vector3D to(0, 0, 1);

        QuaternionD q(from, to);

        Vector3D axis = q.Axis();
        double angle = q.Angle();

        EXPECT_DOUBLE_EQ(0.0, axis.x);
        EXPECT_DOUBLE_EQ(-1.0, axis.y);
        EXPECT_DOUBLE_EQ(0.0, axis.z);
        EXPECT_DOUBLE_EQ(PI<double>() / 2.0, angle);
    }
    {
        Vector3D rotationBasis0(1, 0, 0);
        Vector3D rotationBasis1(0, 0, 1);
        Vector3D rotationBasis2(0, -1, 0);

        QuaternionD q(rotationBasis0, rotationBasis1, rotationBasis2);

        EXPECT_DOUBLE_EQ(std::sqrt(2.0) / 2.0, q.w);
        EXPECT_DOUBLE_EQ(std::sqrt(2.0) / 2.0, q.x);
        EXPECT_DOUBLE_EQ(0.0, q.y);
        EXPECT_DOUBLE_EQ(0.0, q.z);
    }
}

TEST(Quaternion, BasicSetters)
{
    {
        QuaternionD q;
        q.Set(QuaternionD(1, 2, 3, 4));

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(2.0, q.x);
        EXPECT_DOUBLE_EQ(3.0, q.y);
        EXPECT_DOUBLE_EQ(4.0, q.z);
    }
    {
        QuaternionD q;
        q.Set(1, 2, 3, 4);

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(2.0, q.x);
        EXPECT_DOUBLE_EQ(3.0, q.y);
        EXPECT_DOUBLE_EQ(4.0, q.z);
    }
    {
        QuaternionD q;
        q.Set({ 1, 2, 3, 4 });

        EXPECT_DOUBLE_EQ(1.0, q.w);
        EXPECT_DOUBLE_EQ(2.0, q.x);
        EXPECT_DOUBLE_EQ(3.0, q.y);
        EXPECT_DOUBLE_EQ(4.0, q.z);
    }
    {
        // set with axis & angle
        Vector3D originalAxis = Vector3D(1, 3, 2).Normalized();
        double originalAngle = 0.4;

        QuaternionD q;
        q.Set(originalAxis, originalAngle);

        Vector3D axis = q.Axis();
        double angle = q.Angle();

        EXPECT_DOUBLE_EQ(originalAxis.x, axis.x);
        EXPECT_DOUBLE_EQ(originalAxis.y, axis.y);
        EXPECT_DOUBLE_EQ(originalAxis.z, axis.z);
        EXPECT_DOUBLE_EQ(originalAngle, angle);
    }

    {
        // set with from & to vectors (90 degrees)
        Vector3D from(1, 0, 0);
        Vector3D to(0, 0, 1);

        QuaternionD q;
        q.Set(from, to);

        Vector3D axis = q.Axis();
        double angle = q.Angle();

        EXPECT_DOUBLE_EQ(0.0, axis.x);
        EXPECT_DOUBLE_EQ(-1.0, axis.y);
        EXPECT_DOUBLE_EQ(0.0, axis.z);
        EXPECT_DOUBLE_EQ(PI<double>() / 2.0, angle);
    }
    {
        Vector3D rotationBasis0(1, 0, 0);
        Vector3D rotationBasis1(0, 0, 1);
        Vector3D rotationBasis2(0, -1, 0);

        QuaternionD q;
        q.Set(rotationBasis0, rotationBasis1, rotationBasis2);

        EXPECT_DOUBLE_EQ(std::sqrt(2.0) / 2.0, q.w);
        EXPECT_DOUBLE_EQ(std::sqrt(2.0) / 2.0, q.x);
        EXPECT_DOUBLE_EQ(0.0, q.y);
        EXPECT_DOUBLE_EQ(0.0, q.z);
    }
}

TEST(Quaternion, CastTo)
{
    QuaternionD qd(1, 2, 3, 4);
    QuaternionF qf = qd.CastTo<float>();

    EXPECT_FLOAT_EQ(1.f, qf.w);
    EXPECT_FLOAT_EQ(2.f, qf.x);
    EXPECT_FLOAT_EQ(3.f, qf.y);
    EXPECT_FLOAT_EQ(4.f, qf.z);
}

TEST(Quaternion, Normalized)
{
    QuaternionD q(1, 2, 3, 4);
    QuaternionD qn = q.Normalized();

    double denom = std::sqrt(30.0);
    EXPECT_DOUBLE_EQ(1.0 / denom, qn.w);
    EXPECT_DOUBLE_EQ(2.0 / denom, qn.x);
    EXPECT_DOUBLE_EQ(3.0 / denom, qn.y);
    EXPECT_DOUBLE_EQ(4.0 / denom, qn.z);
}

TEST(Quaternion, BinaryOperators)
{
    QuaternionD q1(1, 2, 3, 4);
    QuaternionD q2(1, -2, -3, -4);

    QuaternionD q3 = q1.Mul(q2);

    EXPECT_DOUBLE_EQ(30.0, q3.w);
    EXPECT_DOUBLE_EQ(0.0, q3.x);
    EXPECT_DOUBLE_EQ(0.0, q3.y);
    EXPECT_DOUBLE_EQ(0.0, q3.z);

    q1.Normalize();
    Vector3D v(7, 8, 9);
    Vector3D ans1 = q1.Mul(v);

    Matrix3x3D m = q1.Matrix3();
    Vector3D ans2 = m * v;

    EXPECT_DOUBLE_EQ(ans2.x, ans1.x);
    EXPECT_DOUBLE_EQ(ans2.y, ans1.y);
    EXPECT_DOUBLE_EQ(ans2.z, ans1.z);

    q1.Set(1, 2, 3, 4);
    q2.Set(5, 6, 7, 8);
    EXPECT_DOUBLE_EQ(70.0, q1.Dot(q2));

    q3 = q1.Mul(q2);
    EXPECT_EQ(q3, q2.RMul(q1));
    q1.IMul(q2);
    EXPECT_EQ(q3, q1);
}

TEST(Quaternion, Slerp)
{
    QuaternionD q1( 1.0f, 0.0f, 1.0f, 0.0f);
    QuaternionD q2(-1.0f, 0.0f, 1.0f, 0.0f);

    {
        QuaternionD q = Slerp(q1, q2, 0.0);

        EXPECT_DOUBLE_EQ(q1.x, q.x);
        EXPECT_DOUBLE_EQ(q1.y, q.y);
        EXPECT_DOUBLE_EQ(q1.z, q.z);
        EXPECT_DOUBLE_EQ(q1.w, q.w);
    }
    {
        QuaternionD q = Slerp(q1, q2, 1.0);

        EXPECT_DOUBLE_EQ(q2.x, q.x);
        EXPECT_DOUBLE_EQ(q2.y, q.y);
        EXPECT_DOUBLE_EQ(q2.z, q.z);
        EXPECT_DOUBLE_EQ(q2.w, q.w);
    }
    {
        QuaternionD q = Slerp(q1, q2, 0.5);

        EXPECT_DOUBLE_EQ(0.0, q.x);
        EXPECT_DOUBLE_EQ(1.414213562373095, q.y);
        EXPECT_DOUBLE_EQ(0.0, q.z);
        EXPECT_DOUBLE_EQ(0.0, q.w);
    }
}

TEST(Quaternion, Modifiers)
{
    QuaternionD q(4, 3, 2, 1);
    q.SetIdentity();

    EXPECT_DOUBLE_EQ(1.0, q.w);
    EXPECT_DOUBLE_EQ(0.0, q.x);
    EXPECT_DOUBLE_EQ(0.0, q.y);
    EXPECT_DOUBLE_EQ(0.0, q.z);

    q.Set(4, 3, 2, 1);
    q.Normalize();

    double denom = std::sqrt(30.0);
    EXPECT_DOUBLE_EQ(4.0 / denom, q.w);
    EXPECT_DOUBLE_EQ(3.0 / denom, q.x);
    EXPECT_DOUBLE_EQ(2.0 / denom, q.y);
    EXPECT_DOUBLE_EQ(1.0 / denom, q.z);

    Vector3D axis;
    double angle;
    q.GetAxisAngle(&axis, &angle);
    q.Rotate(1.0);
    double newAngle;
    q.GetAxisAngle(&axis, &newAngle);

    EXPECT_DOUBLE_EQ(angle + 1.0, newAngle);
}

TEST(Quaternion, ComplexGetters)
{
    QuaternionD q(1, 2, 3, 4);

    QuaternionD q2 = q.Inverse();
    EXPECT_DOUBLE_EQ(1.0 / 30.0, q2.w);
    EXPECT_DOUBLE_EQ(-1.0 / 15.0, q2.x);
    EXPECT_DOUBLE_EQ(-1.0 / 10.0, q2.y);
    EXPECT_DOUBLE_EQ(-2.0 / 15.0, q2.z);

    q.Set(1, 0, 5, 2);
    q.Normalize();

    Matrix3x3D mat3 = q.Matrix3();
    double solution3[9] = { -14.0 / 15.0, -2.0 / 15.0, 1.0 / 3.0,
                            2.0 / 15.0,   11.0 / 15.0, 2.0 / 3.0,
                            -1.0 / 3.0,   2.0 / 3.0,   -2.0 / 3.0 };

    for (int i = 0; i < 9; ++i)
    {
        EXPECT_DOUBLE_EQ(solution3[i], mat3[i]);
    }

    Matrix4x4D mat4 = q.Matrix4();
    double solution4[16] = { -14.0 / 15.0, -2.0 / 15.0, 1.0 / 3.0,  0.0,
                             2.0 / 15.0,   11.0 / 15.0, 2.0 / 3.0,  0.0,
                             -1.0 / 3.0,   2.0 / 3.0,   -2.0 / 3.0, 0.0,
                             0.0,          0.0,         0.0,        1.0 };

    Vector3D axis;
    double angle;
    q.GetAxisAngle(&axis, &angle);

    EXPECT_DOUBLE_EQ(0.0, axis.x);
    EXPECT_DOUBLE_EQ(5.0 / std::sqrt(29.0), axis.y);
    EXPECT_DOUBLE_EQ(2.0 / std::sqrt(29.0), axis.z);

    EXPECT_EQ(axis, q.Axis());
    EXPECT_EQ(angle, q.Angle());

    EXPECT_DOUBLE_EQ(2.0 * std::acos(1.0 / std::sqrt(30.0)), angle);

    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(solution4[i], mat4[i]);
    }

    q.Set(1, 2, 3, 4);
    EXPECT_DOUBLE_EQ(std::sqrt(30.0), q.L2Norm());
}

TEST(Quaternion, SetterOperators)
{
    QuaternionD q(1, 2, 3, 4);
    QuaternionD q2;

    q2 = q;
    EXPECT_EQ(1.0, q2.w);
    EXPECT_EQ(2.0, q2.x);
    EXPECT_EQ(3.0, q2.y);
    EXPECT_EQ(4.0, q2.z);

    q2.Set(5, 6, 7, 8);

    q *= q2;

    EXPECT_EQ(-60.0, q.w);
    EXPECT_EQ(12.0, q.x);
    EXPECT_EQ(30.0, q.y);
    EXPECT_EQ(24.0, q.z);
}

TEST(Quaternion, GetterOperators)
{
    QuaternionD q(1, 2, 3, 4);

    EXPECT_EQ(1.0, q[0]);
    EXPECT_EQ(2.0, q[1]);
    EXPECT_EQ(3.0, q[2]);
    EXPECT_EQ(4.0, q[3]);

    QuaternionD q2(1, 2, 3, 4);
    EXPECT_TRUE(q == q2);

    q[0] = 5.0;
    q[1] = 6.0;
    q[2] = 7.0;
    q[3] = 8.0;

    EXPECT_EQ(5.0, q[0]);
    EXPECT_EQ(6.0, q[1]);
    EXPECT_EQ(7.0, q[2]);
    EXPECT_EQ(8.0, q[3]);

    EXPECT_TRUE(q != q2);
}