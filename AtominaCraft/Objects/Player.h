#pragma once
#include "PhysicalGameObject.h"

class Player : public PhysicalGameObject {
public:
	Player();
	virtual ~Player() override {}

	virtual void Reset() override;
	virtual void Update() override;
	virtual void OnCollide(GameObject& other, const Vector3& push) override;

	void Look(float mouseDx, float mouseDy);
	void Move(float moveF, float moveL, float moveUp);

	Matrix4 WorldToCam();
	Matrix4 CamToWorld();
	Vector3 CamOffset();
	Matrix4 LocalToWorld() override {
		return
			Matrix4::Translated(pos) *
			Matrix4::RotatedY(euler.y) *
			Matrix4::RotatedX(euler.x) *
			Matrix4::RotatedZ(euler.z) *
			Matrix4::Scaled(Vector3::Ones());
	}

	Matrix4 WorldToLocal() override {
		return
			Matrix4::Scaled(1.0f / Vector3::Ones()) *
			Matrix4::RotatedZ(-euler.z) *
			Matrix4::RotatedX(-euler.x) *
			Matrix4::RotatedY(-euler.y) *
			Matrix4::Translated(-pos);
	}

	float WalkAcceleration = 10.0f;
	float WalkSpeed = 2.7f;

	float cam_rx;
	float cam_ry;
private:
	float move_f;
	float move_l;
	float move_up;


	float bob_mag;
	float bob_phi;

	bool onGround;
};
