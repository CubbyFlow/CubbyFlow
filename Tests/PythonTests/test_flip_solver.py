import pyCubbyFlow
from pytest import approx
from pytest_utils import *


def test_flip_solver3_init():
    a = pyCubbyFlow.FLIPSolver3()
    assert a.resolution == (1, 1, 1)
    assert a.gridSpacing == (1.0, 1.0, 1.0)
    assert a.gridOrigin == (0.0, 0.0, 0.0)

    b = pyCubbyFlow.FLIPSolver3((2, 3, 4), (5, 6, 7), (8, 9, 10))
    assert b.resolution == (2, 3, 4)
    assert b.gridSpacing == (5.0, 6.0, 7.0)
    assert b.gridOrigin == (8.0, 9.0, 10.0)

    c = pyCubbyFlow.FLIPSolver3(resolution=(2, 3, 4), gridSpacing=(
        5, 6, 7), gridOrigin=(8, 9, 10))
    assert c.resolution == (2, 3, 4)
    assert c.gridSpacing == (5.0, 6.0, 7.0)
    assert c.gridOrigin == (8.0, 9.0, 10.0)


def test_flip_solver3_animation():
    a = pyCubbyFlow.FLIPSolver3()
    f = pyCubbyFlow.Frame()
    a.Update(f)
    assert a.currentFrame.index == 0
    f.Advance()
    a.Update(f)
    assert a.currentFrame.index == 1


def test_flip_solver3_physics_animation():
    a = pyCubbyFlow.FLIPSolver3()
    a.isUsingFixedSubTimeSteps = False
    assert not a.isUsingFixedSubTimeSteps
    a.isUsingFixedSubTimeSteps = True
    assert a.isUsingFixedSubTimeSteps
    a.numberOfFixedSubTimeSteps = 42
    assert a.numberOfFixedSubTimeSteps == 42
    a.AdvanceSingleFrame()
    assert a.currentFrame.index == 0
    assert a.currentTimeInSeconds == approx(0.0, abs=1e-12)
    a.AdvanceSingleFrame()
    assert a.currentFrame.index == 1
    assert a.currentTimeInSeconds == approx(1.0 / 60.0, abs=1e-12)


def test_flip_solver3_grid_fluid_solver3():
    a = pyCubbyFlow.FLIPSolver3()
    a.gravity = (1.0, 2.0, 3.0)
    assert_vector_similar(a.gravity, (1.0, 2.0, 3.0))
    a.viscosityCoefficient = 0.042
    assert a.viscosityCoefficient == 0.042
    a.maxCFL = 3.0
    assert a.maxCFL == 3.0
    a.ResizeGrid((2, 3, 4), (5, 6, 7), (8, 9, 10))
    assert_vector_similar(a.resolution, (2, 3, 4))
    assert_vector_similar(a.gridSpacing, (5.0, 6.0, 7.0))
    assert_vector_similar(a.gridOrigin, (8.0, 9.0, 10.0))
    ds = pyCubbyFlow.GridForwardEulerDiffusionSolver3()
    a.diffusionSolver = ds
    assert ds == a.diffusionSolver


def test_flip_solver3():
    a = pyCubbyFlow.FLIPSolver3()
    a.picBlendingFactor = 0.7
    assert a.picBlendingFactor == 0.7
