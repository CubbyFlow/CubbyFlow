import pyCubbyFlow


def test_parameters():
    data = pyCubbyFlow.SPHSystemData2()

    data.targetDensity = 123.0
    data.targetSpacing = 0.549
    data.relativeKernelRadius = 2.5

    assert data.targetDensity == 123.0
    assert data.targetSpacing == 0.549
    assert data.radius == 0.549
    assert data.relativeKernelRadius == 2.5
    assert data.kernelRadius == 2.5 * 0.549

    data.kernelRadius = 1.9
    assert data.kernelRadius == 1.9
    assert data.targetSpacing == 1.9 / 2.5

    data.radius = 0.413
    assert data.targetSpacing == 0.413
    assert data.radius == 0.413
    assert data.relativeKernelRadius == 2.5
    assert data.kernelRadius == 2.5 * 0.413

    data.mass = 2.0 * data.mass
    assert data.targetDensity == 246.0
