#include "PhysicalGameObject.h"
#include "../GameHeader.h"

PhysicalGameObject::PhysicalGameObject() {
  Reset();
}

void PhysicalGameObject::Reset() {
    GameObject::Reset();
    SetVelocity(Axis::Zero());
    SetScale(1, 1, 1);
    gravity.Set(0.0f, GH_GRAVITY, 0.0f);
    mass = 1;
    bounce = 0.0f;
    friction = 0.02f;
    high_friction = 0.0f;
    drag = 0.008f;
    useGravity = true;
    useCollisions = true;
    prev_pos.SetZero();
}

void PhysicalGameObject::Update() {
    if (useGravity)
        SetVelocity(velocity + gravity * GH_DELTATIME);
    SetVelocity(velocity * (1.0f - drag));
    SetPosition(pos + velocity * GH_DELTATIME);
}

void PhysicalGameObject::OnCollide(GameObject& other, const Vector3& push) {
    //Update position to avoid collision
    //pos += push;

    //Ignore push if delta is too small
    //if (push.MagSq() < 1e-8f) {
    //    return;
    //}
    //
    ////Calculate kinetic friction
    //float kinetic_friction = friction;
    //if (high_friction > 0.0f) {
    //    const float vel_ratio = velocity.Mag() / (high_friction);
    //    kinetic_friction = GH_MIN(friction * (vel_ratio + 5.0f) / (vel_ratio + 1.0f), 1.0f);
    //}
    //
    ////Update velocity to react to collision
    //const Vector3 push_proj = push * (velocity.Dot(push) / push.Dot(push));
    //SetVelocity((velocity - push_proj) * (1.0f - kinetic_friction) - push_proj * bounce);
}