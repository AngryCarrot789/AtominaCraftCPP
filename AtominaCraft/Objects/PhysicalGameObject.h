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

    void SetPositionFromCenter(const Vector3& _pos) {
        SetPosition(_pos);
        prev_pos = _pos;
    }

    void SetVelocity(Vector3 vel) {
        velocity = vel;
        UpdateAABB();
    }

    void SetPosition(Point _pos) override {
        pos = _pos;
        UpdateAABB();
    }
    void SetPosition(float x, float y, float z) override {
        pos = Vector3(x, y, z);
        UpdateAABB();
    }
    void SetScale(Size size) override {
        scale = size;
        UpdateAABB();
    }
    virtual void SetScale(float x, float y, float z) {
        scale = Vector3(x, y, z);
        UpdateAABB();
    }
    void UpdateAABB() {
        collider.SetPositionFromCenter(pos, scale);
    }

    virtual PhysicalGameObject* AsPhysicalGameObject() override { return this; }

    AxisAlignedBB collider;

    Vector3 gravity;
    Vector3 velocity;
    float mass;
    float bounce;
    float friction;
    float high_friction;
    float drag;
    bool useGravity;
    bool useCollisions;

    Point prev_pos;
};
