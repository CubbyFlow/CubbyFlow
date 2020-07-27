import pyCubbyFlow
from pytest import approx


def test_vector2f_Init():
    a = pyCubbyFlow.Vector2F()
    assert a.x == 0.0
    assert a.y == 0.0
    b = pyCubbyFlow.Vector2F(1, 2)
    assert b.x == 1.0
    assert b.y == 2.0
    c = pyCubbyFlow.Vector2F(y=2, x=1)
    assert c.x == 1.0
    assert c.y == 2.0


def test_vector2f_getters():
    a = pyCubbyFlow.Vector2F(1, 2)
    assert a[0] == 1
    assert a[1] == 2


def test_vector2f_setters():
    a = pyCubbyFlow.Vector2F(1, 2)
    a[0] = 4
    a[1] = 5
    assert a[0] == 4
    assert a[1] == 5


def test_vector2f_calc():
    a = pyCubbyFlow.Vector2F(1, 2)
    b = pyCubbyFlow.Vector2F(4, 6)
    c = a + b
    assert c.x == 5.0
    assert c.y == 8.0
    c = a - b
    assert c.x == -3.0
    assert c.y == -4.0
    c = a * b
    assert c.x == 4.0
    assert c.y == 12.0
    c = a / b
    assert c.x == approx(1.0 / 4.0)
    assert c.y == approx(1.0 / 3.0)


def test_vector2d_init():
    a = pyCubbyFlow.Vector2D()
    assert a.x == 0.0
    assert a.y == 0.0
    b = pyCubbyFlow.Vector2D(1, 2)
    assert b.x == 1.0
    assert b.y == 2.0
    c = pyCubbyFlow.Vector2D(y=2, x=1)
    assert c.x == 1.0
    assert c.y == 2.0


def test_vector2d_getters():
    a = pyCubbyFlow.Vector2D(1, 2)
    assert a[0] == 1
    assert a[1] == 2


def test_vector2d_setters():
    a = pyCubbyFlow.Vector2D(1, 2)
    a[0] = 4
    a[1] = 5
    assert a[0] == 4
    assert a[1] == 5


def test_vector2d_calc():
    a = pyCubbyFlow.Vector2D(1, 2)
    b = pyCubbyFlow.Vector2D(4, 6)
    c = a + b
    assert c.x == 5.0
    assert c.y == 8.0
    c = a - b
    assert c.x == -3.0
    assert c.y == -4.0
    c = a * b
    assert c.x == 4.0
    assert c.y == 12.0
    c = a / b
    assert c.x == approx(1.0 / 4.0)
    assert c.y == approx(1.0 / 3.0)


def test_vector3f_init():
    a = pyCubbyFlow.Vector3F()
    assert a.x == 0.0
    assert a.y == 0.0
    assert a.z == 0.0
    b = pyCubbyFlow.Vector3F(1, 2, 3)
    assert b.x == 1.0
    assert b.y == 2.0
    assert b.z == 3.0
    c = pyCubbyFlow.Vector3F(y=2, x=1, z=3)
    assert c.x == 1.0
    assert c.y == 2.0
    assert c.z == 3.0


def test_vector3f_getters():
    a = pyCubbyFlow.Vector3F(1, 2, 3)
    assert a[0] == 1
    assert a[1] == 2
    assert a[2] == 3


def test_vector3f_setters():
    a = pyCubbyFlow.Vector3F(1, 2, 3)
    a[0] = 4
    a[1] = 5
    a[2] = 6
    assert a[0] == 4
    assert a[1] == 5
    assert a[2] == 6


def test_vector3f_calc():
    a = pyCubbyFlow.Vector3F(1, 2, 3)
    b = pyCubbyFlow.Vector3F(4, 6, 8)
    c = a + b
    assert c.x == 5.0
    assert c.y == 8.0
    assert c.z == 11.0
    c = a - b
    assert c.x == -3.0
    assert c.y == -4.0
    assert c.z == -5.0
    c = a * b
    assert c.x == 4.0
    assert c.y == 12.0
    assert c.z == 24.0
    c = a / b
    assert c.x == approx(1.0 / 4.0)
    assert c.y == approx(1.0 / 3.0)
    assert c.z == approx(3.0 / 8.0)


def test_vector3d_Init():
    a = pyCubbyFlow.Vector3D()
    assert a.x == 0.0
    assert a.y == 0.0
    assert a.z == 0.0
    b = pyCubbyFlow.Vector3D(1, 2, 3)
    assert b.x == 1.0
    assert b.y == 2.0
    assert b.z == 3.0
    c = pyCubbyFlow.Vector3D(y=2, x=1, z=3)
    assert c.x == 1.0
    assert c.y == 2.0
    assert c.z == 3.0


def test_vector3d_getters():
    a = pyCubbyFlow.Vector3D(1, 2, 3)
    assert a[0] == 1
    assert a[1] == 2
    assert a[2] == 3


def test_vector3d_setters():
    a = pyCubbyFlow.Vector3D(1, 2, 3)
    a[0] = 4
    a[1] = 5
    a[2] = 6
    assert a[0] == 4
    assert a[1] == 5
    assert a[2] == 6


def test_vector3d_calc():
    a = pyCubbyFlow.Vector3D(1, 2, 3)
    b = pyCubbyFlow.Vector3D(4, 6, 8)
    c = a + b
    assert c.x == 5.0
    assert c.y == 8.0
    assert c.z == 11.0
    c = a - b
    assert c.x == -3.0
    assert c.y == -4.0
    assert c.z == -5.0
    c = a * b
    assert c.x == 4.0
    assert c.y == 12.0
    assert c.z == 24.0
    c = a / b
    assert c.x == approx(1.0 / 4.0)
    assert c.y == approx(1.0 / 3.0)
    assert c.z == approx(3.0 / 8.0)
