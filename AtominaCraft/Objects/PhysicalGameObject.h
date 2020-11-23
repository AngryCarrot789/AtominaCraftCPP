#pragma once
#include "GameObject.h"
#include "../Collisions/AxisAlignedBB.h"

class PhysicalGameObject : public GameObject {
public:
    PhysicalGameObject();
    virtual ~PhysicalGameObject() override {}

    virtual void Reset() override;
    virtual void Update() override;
    virtual void OnCollide(GameObject& other, const Vector3& push);

    void SetFromCenter(const Vector3& _pos) {
        SetPosition(_pos);
        prev_pos = _pos;
    }

    void SetVelocity(Vector3 vel) {
        velocity = vel;
    }

    void SetPosition(Point _pos) override {
        pos = _pos;
        collider.SetFromCenter(pos, scale);
    }
    void SetPosition(float x, float y, float z) override {
        pos = Vector3(x, y, z);
        collider.SetFromCenter(pos, scale);
    }

    virtual PhysicalGameObject* AsPhysicalGameObject() override { return this; }

    AxisAlignedBB collider;

    Vector3 gravity;
    Vector3 velocity;
    float bounce;
    float friction;
    float high_friction;
    float drag;
    bool useGravity;

    Point prev_pos;
};
