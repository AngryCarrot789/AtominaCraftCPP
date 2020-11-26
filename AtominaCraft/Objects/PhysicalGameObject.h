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
        prev_pos = pos;
        SetPosition(_pos);
    }

    void SetVelocity(Axis vel) {
        velocity = vel;
        UpdateAABB();
    }

    void SetPosition(Point _pos) override {
        prev_pos = pos;
        pos = _pos;
        UpdateAABB();
    }
    void SetPosition(float x, float y, float z) override {
        prev_pos = pos;
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

    Axis gravity;
    Axis velocity;
    float mass;
    float bounce;
    float friction;
    float high_friction;
    float drag;
    bool useGravity;
    bool useCollisions;

    Point prev_pos;

    Point PosDifference() {
        return pos - prev_pos;
    }
};
