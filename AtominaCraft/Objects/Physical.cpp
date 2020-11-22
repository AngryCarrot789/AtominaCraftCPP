#include "Physical.h"
#include "../GameHeader.h"

Physical::Physical() {
  Reset();
}

void Physical::Reset() {
  Object::Reset();
  velocity.SetZero();
  gravity.Set(0.0f, GH_GRAVITY, 0.0f);
  bounce = 0.0f;
  friction = 0.0f;
  high_friction = 0.0f;
  drag = 0.0f;
  useGravity = true;
  prev_pos.SetZero();
}

void Physical::Update() {
  prev_pos = pos;
  if (useGravity)
      velocity += gravity * GH_DELTATIME;
  velocity *= (1.0f - drag);
  pos += velocity * GH_DELTATIME;
}

void Physical::OnCollide(Object& other, const Vector3& push) {
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
    //velocity = (velocity - push_proj) * (1.0f - kinetic_friction) - push_proj * bounce;
}