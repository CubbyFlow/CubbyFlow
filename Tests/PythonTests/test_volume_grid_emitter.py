import numpy as np
import pyCubbyFlow
from pytest_utils import *


def test_volume_grid_emitter2():
    # Basic ctor test
    sphere = pyCubbyFlow.Sphere2(center=(0.5, 0.5), radius=0.15)
    emitter = pyCubbyFlow.VolumeGridEmitter2(sphere, False)

    assert emitter.sourceRegion
    assert not emitter.isOneShot
    assert emitter.isEnabled

    # Another basic ctor test
    emitter2 = pyCubbyFlow.VolumeGridEmitter2(
        sourceRegion=sphere, isOneShot=False)

    assert emitter2.sourceRegion
    assert not emitter2.isOneShot
    assert emitter2.isEnabled

    # One-shot emitter
    emitter3 = pyCubbyFlow.VolumeGridEmitter2(
        sourceRegion=sphere, isOneShot=True)

    assert emitter3.isOneShot

    frame = pyCubbyFlow.Frame()
    solver = pyCubbyFlow.GridSmokeSolver2(resolution=(32, 32), domainSizeX=1.0)
    solver.emitter = emitter3
    emitter3.AddStepFunctionTarget(solver.smokeDensity, 0.0, 1.0)
    emitter3.AddStepFunctionTarget(solver.temperature, 0.0, 1.0)

    # Emit some smoke
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    solver.Update(frame)
    frame.Advance()
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff > 0.0
    assert not emitter3.isEnabled

    # Should not emit more smoke
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    emitter3.Update(0, 0)
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff < 1e-20

    # Re-enabling the emitter should make it emit one more time
    emitter3.isEnabled = True
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    solver.Update(frame)
    frame.Advance()
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff > 0.0
    assert not emitter3.isEnabled

    # ...and gets disabled again
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    emitter3.Update(0, 0)
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff < 1e-20


def test_volume_grid_emitter3():
    # Basic ctor test
    sphere = pyCubbyFlow.Sphere3(center=(0.5, 0.5, 0.5), radius=0.15)
    emitter = pyCubbyFlow.VolumeGridEmitter3(sphere, False)

    assert emitter.sourceRegion
    assert not emitter.isOneShot
    assert emitter.isEnabled

    # Another basic ctor test
    emitter2 = pyCubbyFlow.VolumeGridEmitter3(
        sourceRegion=sphere, isOneShot=False)

    assert emitter2.sourceRegion
    assert not emitter2.isOneShot
    assert emitter2.isEnabled

    # One-shot emitter
    emitter3 = pyCubbyFlow.VolumeGridEmitter3(
        sourceRegion=sphere, isOneShot=True)

    assert emitter3.isOneShot

    frame = pyCubbyFlow.Frame()
    solver = pyCubbyFlow.GridSmokeSolver3(
        resolution=(32, 32, 32), domainSizeX=1.0)
    solver.emitter = emitter3
    emitter3.AddStepFunctionTarget(solver.smokeDensity, 0.0, 1.0)
    emitter3.AddStepFunctionTarget(solver.temperature, 0.0, 1.0)

    # Emit some smoke
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    solver.Update(frame)
    frame.Advance()
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff > 0.0
    assert not emitter3.isEnabled

    # Should not emit more smoke
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    emitter3.Update(0, 0)
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff < 1e-20

    # Re-enabling the emitter should make it emit one more time
    emitter3.isEnabled = True
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    solver.Update(frame)
    frame.Advance()
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff > 0.0
    assert not emitter3.isEnabled

    # ...and gets disabled again
    old_den = np.array(solver.smokeDensity.DataView(), copy=True)
    emitter3.Update(0, 0)
    new_den = np.array(solver.smokeDensity.DataView(), copy=True)
    diff = np.linalg.norm(old_den - new_den)
    assert diff < 1e-20
