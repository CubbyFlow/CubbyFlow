import pyCubbyFlow


def create_collider2():
    sphere = pyCubbyFlow.Sphere2()
    collider = pyCubbyFlow.RigidBodyCollider2(surface=sphere)
    return collider


def create_collider3():
    sphere = pyCubbyFlow.Sphere3()
    collider = pyCubbyFlow.RigidBodyCollider3(surface=sphere)
    return collider


def test_collider_set2():
    collider_set = pyCubbyFlow.ColliderSet2()
    assert collider_set.numberOfColliders == 0

    collider1 = create_collider2()
    collider2 = create_collider2()
    collider3 = create_collider2()
    collider_set.AddCollider(collider1)
    collider_set.AddCollider(collider2)
    collider_set.AddCollider(collider3)
    assert collider_set.numberOfColliders == 3

    assert collider1 == collider_set.Collider(0)
    assert collider2 == collider_set.Collider(1)
    assert collider3 == collider_set.Collider(2)


def test_collider_set3():
    collider_set = pyCubbyFlow.ColliderSet3()
    assert collider_set.numberOfColliders == 0

    collider1 = create_collider3()
    collider2 = create_collider3()
    collider3 = create_collider3()
    collider_set.AddCollider(collider1)
    collider_set.AddCollider(collider2)
    collider_set.AddCollider(collider3)
    assert collider_set.numberOfColliders == 3

    assert collider1 == collider_set.Collider(0)
    assert collider2 == collider_set.Collider(1)
    assert collider3 == collider_set.Collider(2)
