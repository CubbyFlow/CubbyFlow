import math
import pyCubbyFlow
from pytest import approx


def test_init():
    a = pyCubbyFlow.QuaternionD()
    assert a.w == 1.0
    assert a.x == 0.0
    assert a.y == 0.0
    assert a.z == 0.0
    b = pyCubbyFlow.QuaternionD(-1, 2, 3, 4)
    assert b.w == -1.0
    assert b.x == 2.0
    assert b.y == 3.0
    assert b.z == 4.0
    c = pyCubbyFlow.QuaternionD(x=2, z=4, w=1, y=3)
    assert c.w == 1.0
    assert c.x == 2.0
    assert c.y == 3.0
    assert c.z == 4.0


def test_getters():
    a = pyCubbyFlow.QuaternionD(1, 2, 3, 4)
    assert a[0] == a.w
    assert a[1] == a.x
    assert a[2] == a.y
    assert a[3] == a.z
    a.Normalize()
    axis = a.Axis()
    angle = a.Angle()
    denom = math.sqrt(1 - a.w * a.w)
    assert axis.x == approx(math.sqrt(2.0 / 15.0) / denom)
    assert axis.y == approx(math.sqrt(3.0 / 10.0) / denom)
    assert axis.z == approx(2.0 * math.sqrt(2.0 / 15.0) / denom)
    assert angle == approx(2.0 * math.acos(1.0 / math.sqrt(30.0)))


def test_setters():
    a = pyCubbyFlow.QuaternionD()
    a.SetAxisAngle((0, -1, 0), math.pi / 2)
    axis = a.Axis()
    angle = a.Angle()
    assert axis.x == 0
    assert axis.y == -1
    assert axis.z == 0
    assert angle == math.pi / 2
    a[0] = 1.0
    a[1] = 2.0
    a[2] = 3.0
    a[3] = 4.0
    assert 1 == a.w
    assert 2 == a.x
    assert 3 == a.y
    assert 4 == a.z
