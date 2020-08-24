import numpy as np
import pyCubbyFlow


def test_init2():
    ps = pyCubbyFlow.ParticleSystemData2()
    assert ps.numberOfParticles == 0

    ps2 = pyCubbyFlow.ParticleSystemData2(100)
    assert ps2.numberOfParticles == 100


def test_resize2():
    ps = pyCubbyFlow.ParticleSystemData2()
    ps.Resize(12)
    assert ps.numberOfParticles == 12


def test_add_scalar_data2():
    ps = pyCubbyFlow.ParticleSystemData2()
    ps.Resize(12)

    a0 = ps.AddScalarData(2.0)
    a1 = ps.AddScalarData(9.0)
    assert ps.numberOfParticles == 12
    assert a0 == 0
    assert a1 == 1

    as0 = np.array(ps.ScalarDataAt(a0))
    for val in as0:
        assert val == 2.0

    as1 = np.array(ps.ScalarDataAt(a1))
    for val in as1:
        assert val == 9.0


def test_add_vector_data2():
    ps = pyCubbyFlow.ParticleSystemData2()
    ps.Resize(12)

    a0 = ps.AddVectorData((2.0, 4.0))
    a1 = ps.AddVectorData((9.0, -2.0))
    assert ps.numberOfParticles == 12
    assert a0 == 3
    assert a1 == 4

    as0 = np.array(ps.VectorDataAt(a0))
    for val in as0:
        assert val.tolist() == [2.0, 4.0]

    as1 = np.array(ps.VectorDataAt(a1))
    for val in as1:
        assert val.tolist() == [9.0, -2.0]


def test_add_particles2():
    ps = pyCubbyFlow.ParticleSystemData2()
    ps.Resize(12)

    ps.AddParticles([(1.0, 2.0), (4.0, 5.0)],
                    [(7.0, 8.0), (8.0, 7.0)],
                    [(5.0, 4.0), (2.0, 1.0)])

    assert ps.numberOfParticles == 14
    p = np.array(ps.positions)
    v = np.array(ps.velocities)
    f = np.array(ps.forces)

    assert [1.0, 2.0] == p[12].tolist()
    assert [4.0, 5.0] == p[13].tolist()
    assert [7.0, 8.0] == v[12].tolist()
    assert [8.0, 7.0] == v[13].tolist()
    assert [5.0, 4.0] == f[12].tolist()
    assert [2.0, 1.0] == f[13].tolist()


# ------------------------------------------------------------------------------

def test_init3():
    ps = pyCubbyFlow.ParticleSystemData3()
    assert ps.numberOfParticles == 0

    ps2 = pyCubbyFlow.ParticleSystemData3(100)
    assert ps2.numberOfParticles == 100


def test_resize3():
    ps = pyCubbyFlow.ParticleSystemData3()
    ps.Resize(12)
    assert ps.numberOfParticles == 12


def test_add_scalar_data3():
    ps = pyCubbyFlow.ParticleSystemData3()
    ps.Resize(12)

    a0 = ps.AddScalarData(2.0)
    a1 = ps.AddScalarData(9.0)
    assert ps.numberOfParticles == 12
    assert a0 == 0
    assert a1 == 1

    as0 = np.array(ps.ScalarDataAt(a0))
    for val in as0:
        assert val == 2.0

    as1 = np.array(ps.ScalarDataAt(a1))
    for val in as1:
        assert val == 9.0


def test_add_vector_data3():
    ps = pyCubbyFlow.ParticleSystemData3()
    ps.Resize(12)

    a0 = ps.AddVectorData((2.0, 4.0, -1.0))
    a1 = ps.AddVectorData((9.0, -2.0, 5.0))
    assert ps.numberOfParticles == 12
    assert a0 == 3
    assert a1 == 4

    as0 = np.array(ps.VectorDataAt(a0))
    for val in as0:
        assert val.tolist() == [2.0, 4.0, -1.0]

    as1 = np.array(ps.VectorDataAt(a1))
    for val in as1:
        assert val.tolist() == [9.0, -2.0, 5.0]


def test_add_particles3():
    ps = pyCubbyFlow.ParticleSystemData3()
    ps.Resize(12)

    ps.AddParticles([(1.0, 2.0, 3.0), (4.0, 5.0, 6.0)],
                    [(7.0, 8.0, 9.0), (8.0, 7.0, 6.0)],
                    [(5.0, 4.0, 3.0), (2.0, 1.0, 3.0)])

    assert ps.numberOfParticles == 14
    p = np.array(ps.positions)
    v = np.array(ps.velocities)
    f = np.array(ps.forces)

    assert [1.0, 2.0, 3.0] == p[12].tolist()
    assert [4.0, 5.0, 6.0] == p[13].tolist()
    assert [7.0, 8.0, 9.0] == v[12].tolist()
    assert [8.0, 7.0, 6.0] == v[13].tolist()
    assert [5.0, 4.0, 3.0] == f[12].tolist()
    assert [2.0, 1.0, 3.0] == f[13].tolist()
