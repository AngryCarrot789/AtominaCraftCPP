#pragma once
#include "Vector3.h"
#include "Maths.h"

class Quaternion {
public:
	float w;
	// Pitch
	float x;
	// Yaw
	float y;
	// Roll
	float z;

	Quaternion();
	Quaternion(const Quaternion& quaternion);
	Quaternion(Euler euler);
	Quaternion(float _w, float _x, float _y, float _z);

	static Quaternion ToQuaternion(Euler euler) {
		float yaw = euler.y;
		float roll = euler.z;
		float pitch = euler.x;
		float cy = Maths::Cos(yaw / 2);
		float sy = Maths::Sin(yaw / 2);
		float cp = Maths::Cos(pitch / 2);
		float sp = Maths::Cos(pitch / 2);
		float cr = Maths::Cos(roll / 2);
		float sr = Maths::Cos(roll / 2);

		Quaternion q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;
	}

	static Euler ToEuler(Quaternion q) {
		Euler angles;

		// roll (x-axis rotation)
		float sr_cp = 2.0f * (q.w * q.x + q.y * q.z);
		float cr_cp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
		angles.z = std::atan2(sr_cp, cr_cp);

		// pitch (y-axis rotation)
		float sp = 2.0f * (q.w * q.y - q.z * q.x);
		if (std::abs(sp) >= 1.0f)
			angles.x = std::copysign(Maths::HALF_PI, sp); // use 90 degrees if out of range
		else
			angles.x = std::asin(sp);

		// yaw (z-axis rotation)
		float sy_cp = 2.0f * (q.w * q.z + q.x * q.y);
		float cy_cp = 1.0f - 2 * (q.y * q.y + q.z * q.z);
		angles.y = std::atan2(sy_cp, cy_cp);

		return angles;
	}

	static Quaternion RotateAxisZ(Euler euler) {
		float a = std::atan2(euler.x, euler.y);
		return Quaternion(Maths::Cos(a / 2), 0, 0, Maths::Sin(a / 2));
	}

	void Set(Quaternion q) {
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}
	void Set(float _w, float _x, float _y, float _z) {
		w = _w;
		x = _x;
		y = _y;
		z = _z;
	}

	static Quaternion AxisAngles(float rotationDeg, Axis lockingAxis) {
		float a = rotationDeg;
		float s = Maths::Sin(a* 0.5f);
		Euler rot = lockingAxis * s;
		return Quaternion(Maths::Cos(a * 0.5f), rot.x, rot.y, rot.z);
	}

	Vector3 RotateVector(Vector3 coordinate);
	Vector3 UnrotateVector(Vector3 coordinate);
	Vector3 GetBiVector();
	
	Quaternion Add(Quaternion quaternion);
	Quaternion Subtract(Quaternion quaternion);
	Quaternion Multiply(Quaternion quaternion);
	Quaternion Multiply(float scalar);
	Quaternion Divide(Quaternion quaternion);
	Quaternion Divide(float scalar);
	Quaternion Power(Quaternion quaternion);
	
	Quaternion Power(float exponent);
	Quaternion Permutate(Vector3 permutation);
	
	Quaternion Absolute();
	Quaternion AdditiveInverse();
	Quaternion MultiplicativeInverse();
	Quaternion Conjugate();
	Quaternion UnitQuaternion();

	float Magnitude();
	float DotProduct(Quaternion quaternion);
	float Normal();
	
	bool IsNonZero();
	bool IsEqual(Quaternion quaternion);

	//Static functions
	static Quaternion SphericalInterpolation(Quaternion q1, Quaternion q2, float ratio);
	static Quaternion Add(Quaternion q1, Quaternion q2) { return q1.Add(q2); }
	static Quaternion Subtract(Quaternion q1, Quaternion q2) { return q1.Subtract(q2); }
	static Quaternion Multiply(Quaternion q1, Quaternion q2) {
		return q1.Multiply(q2);
	}
	static Quaternion Divide(Quaternion q1, Quaternion q2) {
		return q1.Divide(q2);
	}
	static Quaternion Power(Quaternion q1, Quaternion q2) {
		return q1.Power(q2);
	}
	static float DotProduct(Quaternion q1, Quaternion q2) {
		return q1.DotProduct(q2);
	}
	static Quaternion Power(Quaternion quaternion, float exponent) {
		return quaternion.Power(exponent);
	}
	static Quaternion Permutate(Quaternion quaternion, Vector3 vector) {
		return quaternion.Permutate(vector);
	}
	static Quaternion Absolute(Quaternion quaternion) {
		return quaternion.Absolute();
	}
	static Quaternion AdditiveInverse(Quaternion quaternion) {
		return quaternion.AdditiveInverse();
	}
	static Quaternion MultiplicativeInverse(Quaternion quaternion) {
		return quaternion.MultiplicativeInverse();
	}
	static Quaternion Conjugate(Quaternion quaternion) {
		return quaternion.Conjugate();
	}
	static Quaternion UnitQuaternion(Quaternion quaternion) {
		return quaternion.UnitQuaternion();
	}
	static float Magnitude(Quaternion quaternion) {
		return quaternion.Magnitude();
	}
	static float Normal(Quaternion quaternion) {
		return quaternion.Normal();
	}

	//Operator overloads
	bool operator ==(Quaternion quaternion) {
		return this->IsEqual(quaternion);
	}
	bool operator !=(Quaternion quaternion) {
		return !(this->IsEqual(quaternion));
	}
	Quaternion operator =(Quaternion quaternion) {
		this->w = quaternion.w;
		this->x = quaternion.x;
		this->y = quaternion.y;
		this->z = quaternion.z;
	
		return quaternion;
	}
	Quaternion operator  +(Quaternion quaternion) {
		Quaternion q = Quaternion(*this);
	
		return q.Add(quaternion);
	}
	Quaternion operator  -(Quaternion quaternion) {
		Quaternion q = Quaternion(*this);
	
		return q.Subtract(quaternion);
	}
	Quaternion operator  *(Quaternion quaternion) {
		Quaternion q = Quaternion(*this);
	
		return q.Multiply(quaternion);
	}
	Quaternion operator  /(Quaternion quaternion) {
		Quaternion q = Quaternion(*this);
	
		return q.Divide(quaternion);
	}
	Quaternion operator  /(float value) {
		Quaternion q = Quaternion(*this);
	
		return q.Divide(value);
	}
	friend Quaternion operator *(float scalar, Quaternion q);
	friend Quaternion operator *(Quaternion q, float scalar);
};