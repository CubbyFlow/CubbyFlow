import pyCubbyFlow


def test_bounding_box2f_init():
    a = pyCubbyFlow.BoundingBox2D()
    assert a.lowerCorner.x > a.upperCorner.x
    assert a.lowerCorner.y > a.upperCorner.y

    b = pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2))
    assert b.lowerCorner.x == -1.0
    assert b.lowerCorner.y == -2.0
    assert b.upperCorner.x == 4.0
    assert b.upperCorner.y == 2.0

    l, c = pyCubbyFlow.Vector2D(-1, -2), pyCubbyFlow.Vector2D(4, 2)
    c = pyCubbyFlow.BoundingBox2D(l, c)
    assert c.lowerCorner.x == -1.0
    assert c.lowerCorner.y == -2.0
    assert c.upperCorner.x == 4.0
    assert c.upperCorner.y == 2.0


def test_bounding_box2f_is_empty():
    a = pyCubbyFlow.BoundingBox2D((-2.0, -2.0), (4.0, 3.0))
    assert not a.IsEmpty()


# ------------------------------------------------------------------------------


def test_bounding_box2d_init():
    a = pyCubbyFlow.BoundingBox2D()
    assert a.lowerCorner.x > a.upperCorner.x
    assert a.lowerCorner.y > a.upperCorner.y

    b = pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2))
    assert b.lowerCorner.x == -1.0
    assert b.lowerCorner.y == -2.0
    assert b.upperCorner.x == 4.0
    assert b.upperCorner.y == 2.0

    l, c = pyCubbyFlow.Vector2D(-1, -2), pyCubbyFlow.Vector2D(4, 2)
    c = pyCubbyFlow.BoundingBox2D(l, c)
    assert c.lowerCorner.x == -1.0
    assert c.lowerCorner.y == -2.0
    assert c.upperCorner.x == 4.0
    assert c.upperCorner.y == 2.0


def test_bounding_box2d_is_empty():
    a = pyCubbyFlow.BoundingBox2D((-2.0, -2.0), (4.0, 3.0))
    assert not a.IsEmpty()


# ------------------------------------------------------------------------------


def test_bounding_box3f_init():
    a = pyCubbyFlow.BoundingBox3F()
    assert a.lowerCorner.x > a.upperCorner.x
    assert a.lowerCorner.y > a.upperCorner.y
    assert a.lowerCorner.z > a.upperCorner.z

    b = pyCubbyFlow.BoundingBox3F((-1, -2, -3), (4, 2, 5))
    assert b.lowerCorner.x == -1.0
    assert b.lowerCorner.y == -2.0
    assert b.lowerCorner.z == -3.0
    assert b.upperCorner.x == 4.0
    assert b.upperCorner.y == 2.0
    assert b.upperCorner.z == 5.0

    l, c = pyCubbyFlow.Vector3F(-1, -2, -3), pyCubbyFlow.Vector3F(4, 2, 5)
    c = pyCubbyFlow.BoundingBox3F(l, c)
    assert c.lowerCorner.x == -1.0
    assert c.lowerCorner.y == -2.0
    assert c.lowerCorner.z == -3.0
    assert c.upperCorner.x == 4.0
    assert c.upperCorner.y == 2.0
    assert c.upperCorner.z == 5.0


def test_bounding_box3f_is_empty():
    a = pyCubbyFlow.BoundingBox3F((-2.0, -2.0, 1.0), (4.0, 3.0, 5.0))
    assert not a.IsEmpty()


# ------------------------------------------------------------------------------


def test_bounding_box3d_init():
    a = pyCubbyFlow.BoundingBox3D()
    assert a.lowerCorner.x > a.upperCorner.x
    assert a.lowerCorner.y > a.upperCorner.y
    assert a.lowerCorner.z > a.upperCorner.z

    b = pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 5))
    assert b.lowerCorner.x == -1.0
    assert b.lowerCorner.y == -2.0
    assert b.lowerCorner.z == -3.0
    assert b.upperCorner.x == 4.0
    assert b.upperCorner.y == 2.0
    assert b.upperCorner.z == 5.0

    l, c = pyCubbyFlow.Vector3D(-1, -2, -3), pyCubbyFlow.Vector3D(4, 2, 5)
    c = pyCubbyFlow.BoundingBox3D(l, c)
    assert c.lowerCorner.x == -1.0
    assert c.lowerCorner.y == -2.0
    assert c.lowerCorner.z == -3.0
    assert c.upperCorner.x == 4.0
    assert c.upperCorner.y == 2.0
    assert c.upperCorner.z == 5.0


def test_bounding_box3d_is_empty():
    a = pyCubbyFlow.BoundingBox3D((-2.0, -2.0, 1.0), (4.0, 3.0, 5.0))
    assert not a.IsEmpty()
