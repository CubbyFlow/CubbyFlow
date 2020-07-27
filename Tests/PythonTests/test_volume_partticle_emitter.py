import pyCubbyFlow
from pytest_utils import *


def test_volume_particle_emitter2():
    # Basic ctor test
    sphere = pyCubbyFlow.Sphere2()
    emitter = pyCubbyFlow.VolumeParticleEmitter2(
        sphere,
        pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2)),
        0.1,
        (-1, 0.5),
        (3, 4),
        5.0,
        30,
        0.01,
        False,
        True,
        42)

    assert emitter.surface
    assert_bounding_box_similar(
        emitter.maxRegion, pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2)))
    assert emitter.spacing == 0.1
    assert_vector_similar(emitter.initialVelocity, (-1, 0.5))
    assert_vector_similar(emitter.linearVelocity, (3, 4))
    assert emitter.angularVelocity == 5.0
    assert emitter.maxNumberOfParticles == 30
    assert emitter.jitter == 0.01
    assert not emitter.isOneShot
    assert emitter.allowOverlapping
    assert emitter.isEnabled

    # Another basic ctor test
    emitter2 = pyCubbyFlow.VolumeParticleEmitter2(
        implicitSurface=sphere,
        maxRegion=pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2)),
        spacing=0.1,
        initialVelocity=(-1, 0.5),
        linearVelocity=(3, 4),
        angularVelocity=5.0,
        maxNumberOfParticles=3000,
        jitter=0.01,
        isOneShot=False,
        allowOverlapping=True,
        seed=42)

    assert emitter2.surface
    assert_bounding_box_similar(
        emitter2.maxRegion, pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2)))
    assert emitter2.spacing == 0.1
    assert_vector_similar(emitter2.initialVelocity, (-1, 0.5))
    assert_vector_similar(emitter2.linearVelocity, (3, 4))
    assert emitter2.angularVelocity == 5.0
    assert emitter2.maxNumberOfParticles == 3000
    assert emitter2.jitter == 0.01
    assert not emitter2.isOneShot
    assert emitter2.allowOverlapping
    assert emitter2.isEnabled

    # Emit some particles
    frame = pyCubbyFlow.Frame()
    solver = pyCubbyFlow.ParticleSystemSolver2()
    solver.emitter = emitter2
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > 0

    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > old_num_particles

    # Disabling emitter should stop emitting particles
    emitter2.isEnabled = False
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles == old_num_particles

    # Re-enabling emitter should resume emission
    emitter2.isEnabled = True
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > old_num_particles

    # One-shot emitter
    emitter3 = pyCubbyFlow.VolumeParticleEmitter2(
        implicitSurface=sphere,
        maxRegion=pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2)),
        spacing=0.1,
        initialVelocity=(-1, 0.5),
        linearVelocity=(3, 4),
        angularVelocity=5.0,
        maxNumberOfParticles=3000,
        jitter=0.01,
        isOneShot=True,
        allowOverlapping=True,
        seed=42)

    # Emit some particles
    frame = pyCubbyFlow.Frame()
    solver = pyCubbyFlow.ParticleSystemSolver2()
    solver.emitter = emitter3
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > 0
    assert not emitter3.isEnabled

    # Should not emit more particles
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles == old_num_particles

    # Re-enabling the emitter should make it emit one more time
    emitter3.isEnabled = True
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > old_num_particles

    # ...and gets disabled again
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles == old_num_particles


def test_volume_particle_emitter3():
    sphere = pyCubbyFlow.Sphere3()
    emitter = pyCubbyFlow.VolumeParticleEmitter3(
        sphere,
        pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 9)),
        0.1,
        (-1, 0.5, 2),
        (3, 4, 5),
        (6, 7, 8),
        30,
        0.01,
        False,
        True,
        42)

    assert emitter.surface
    assert_bounding_box_similar(
        emitter.maxRegion, pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 9)))
    assert emitter.spacing == 0.1
    assert_vector_similar(emitter.initialVelocity, (-1, 0.5, 2))
    assert_vector_similar(emitter.linearVelocity, (3, 4, 5))
    assert_vector_similar(emitter.angularVelocity, (6, 7, 8))
    assert emitter.maxNumberOfParticles == 30
    assert emitter.jitter == 0.01
    assert not emitter.isOneShot
    assert emitter.allowOverlapping
    assert emitter.isEnabled

    emitter2 = pyCubbyFlow.VolumeParticleEmitter3(
        implicitSurface=sphere,
        maxRegion=pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 9)),
        spacing=0.1,
        initialVelocity=(-1, 0.5, 2),
        linearVelocity=(3, 4, 5),
        angularVelocity=(6, 7, 8),
        maxNumberOfParticles=300000,
        jitter=0.01,
        isOneShot=False,
        allowOverlapping=True,
        seed=42)

    assert emitter2.surface
    assert_bounding_box_similar(
        emitter2.maxRegion, pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 9)))
    assert emitter2.spacing == 0.1
    assert_vector_similar(emitter2.initialVelocity, (-1, 0.5, 2))
    assert_vector_similar(emitter2.linearVelocity, (3, 4, 5))
    assert_vector_similar(emitter2.angularVelocity, (6, 7, 8))
    assert emitter2.maxNumberOfParticles == 300000
    assert emitter2.jitter == 0.01
    assert not emitter2.isOneShot
    assert emitter2.allowOverlapping
    assert emitter2.isEnabled

    # Emit some particles
    frame = pyCubbyFlow.Frame()
    solver = pyCubbyFlow.ParticleSystemSolver3()
    solver.emitter = emitter2
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > 0

    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > old_num_particles

    # Disabling emitter should stop emitting particles
    emitter2.isEnabled = False
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles == old_num_particles

    # Re-enabling emitter should resume emission
    emitter2.isEnabled = True
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > old_num_particles

    # One-shot emitter
    emitter3 = pyCubbyFlow.VolumeParticleEmitter3(
        implicitSurface=sphere,
        maxRegion=pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 9)),
        spacing=0.1,
        initialVelocity=(-1, 0.5, 2),
        linearVelocity=(3, 4, 5),
        angularVelocity=(6, 7, 8),
        maxNumberOfParticles=300000,
        jitter=0.01,
        isOneShot=True,
        allowOverlapping=True,
        seed=42)

    # Emit some particles
    frame = pyCubbyFlow.Frame()
    solver = pyCubbyFlow.ParticleSystemSolver3()
    solver.emitter = emitter3
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > 0
    assert not emitter3.isEnabled

    # Should not emit more particles
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles == old_num_particles

    # Re-enabling the emitter should make it emit one more time
    emitter3.isEnabled = True
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles > old_num_particles

    # ...and gets disabled again
    old_num_particles = solver.particleSystemData.numberOfParticles
    solver.Update(frame)
    frame.Advance()
    assert solver.particleSystemData.numberOfParticles == old_num_particles
