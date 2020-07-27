import pyCubbyFlow


def test_face_centered_grid2_Fill():
    a = pyCubbyFlow.FaceCenteredGrid2((10, 10))
    a.Fill((3.0, 4.0))
    for j in range(10):
        for i in range(11):
            assert a.GetU(i, j) == 3.0
    for j in range(11):
        for i in range(10):
            assert a.GetV(i, j) == 4.0

    def filler(pt):
        return (pt.x, pt.y)

    a.Fill(filler)
    for j in range(10):
        for i in range(11):
            a.GetU(i, j) == i
    for j in range(11):
        for i in range(10):
            a.GetV(i, j) == j


def test_face_centered_grid2_for_each():
    a = pyCubbyFlow.FaceCenteredGrid2((10, 10))
    # Workaround for Python 2.x which doesn't support nonlocal
    d = {'ei': 0, 'ej': 0}

    def checkU(i, j):
        i == d['ei']
        j == d['ej']
        d['ei'] += 1
        if d['ei'] >= 11:
            d['ei'] = 0
            d['ej'] += 1

    a.ForEachUIndex(checkU)
    d = {'ei': 0, 'ej': 0}

    def checkV(i, j):
        i == d['ei']
        j == d['ej']
        d['ei'] += 1
        if d['ei'] >= 10:
            d['ei'] = 0
            d['ej'] += 1

    a.ForEachVIndex(checkV)


def test_face_centered_grid2_serialization():
    a = pyCubbyFlow.FaceCenteredGrid2((10, 10))

    def filler(pt):
        return (pt.x, pt.y)

    a.Fill(filler)

    flatBuffer = a.Serialize()

    b = pyCubbyFlow.FaceCenteredGrid2()
    b.Deserialize(flatBuffer)

    for j in range(10):
        for i in range(11):
            b.GetU(i, j) == i
    for j in range(11):
        for i in range(10):
            b.GetV(i, j) == i
