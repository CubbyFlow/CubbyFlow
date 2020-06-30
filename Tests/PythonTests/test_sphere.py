import pyCubbyFlow


def test_init():
    a = pyCubbyFlow.Sphere3()
    assert a.isNormalFlipped == False
    assert a.center == (0, 0, 0)
    assert a.radius == 1.0
