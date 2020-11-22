#pragma once
#include "Object.h"
#include "../Collision/Sphere.h"

class Physical : public Object {
public:
  Physical();
  virtual ~Physical() override {}

  virtual void Reset() override;
  virtual void Update() override;
  virtual void OnCollide(Object& other, const Vector3& push);

  void SetFromCenter(const Vector3& _pos) {
    pos = _pos;
    prev_pos = _pos;
  }

  virtual Physical* AsPhysical() override { return this; }

  Vector3 gravity;
  Vector3 velocity;
  float bounce;
  float friction;
  float high_friction;
  float drag;
  bool useGravity;
  
  Vector3 prev_pos;

  std::vector<Sphere> hitSpheres;
};
