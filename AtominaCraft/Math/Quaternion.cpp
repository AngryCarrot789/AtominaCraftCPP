
#include "Quaternion.h"

Quaternion::Quaternion() {
	this->w = 1.0;
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

Quaternion::Quaternion(const Quaternion& quaternion) {
	this->w = quaternion.w;
	this->x = quaternion.x;
	this->y = quaternion.y;
	this->z = quaternion.z;
}

Quaternion::Quaternion(Euler vector) {
	this->w = 0;
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
}

Quaternion::Quaternion(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Quaternion::RotateVector(Vector3 coordinate) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);
	Quaternion qv = Quaternion(0, coordinate.x, coordinate.y, coordinate.z);
	Quaternion qr = current * qv * current.MultiplicativeInverse();

	return Vector3{
		qr.x,
		qr.y,
		qr.z
	};
}

Vector3 Quaternion::UnrotateVector(Vector3 coordinate) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return current.Conjugate().RotateVector(coordinate);
}

Vector3 Quaternion::GetBiVector() {
	return Vector3{
		this->x,
		this->y,
		this->z
	};
}

Quaternion Quaternion::SphericalInterpolation(Quaternion q1, Quaternion q2, float ratio) {
	q1 = q1.UnitQuaternion();
	q2 = q2.UnitQuaternion();

	float dot = q1.DotProduct(q2);//Cosine between the two quaternions

	if (dot < 0.0)//Shortest path correction
	{
		q1 = q1.AdditiveInverse();
		dot = -dot;
	}

	if (dot > 0.999)//Linearly interpolates if results are close
	{
		return (q1.Add((q2.Subtract(q1)).Multiply(ratio))).UnitQuaternion();
	}
	else
	{
		dot = Maths::Clamp(dot, -1, 1);

		float theta0 = acos(dot);
		float theta = theta0 * ratio;

		//Quaternion q3 = (q2.Subtract(q1.Multiply(dot))).UnitQuaternion();//UQ for orthonomal 
		float f1 = cos(theta) - dot * sin(theta) / sin(theta0);
		float f2 = sin(theta) / sin(theta0);

		return q1.Multiply(f1).Add(q2.Multiply(f2)).UnitQuaternion();
	}
}

Quaternion Quaternion::Add(Quaternion quaternion) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion{
		current.w + quaternion.w,
		current.x + quaternion.x,
		current.y + quaternion.y,
		current.z + quaternion.z
	};
}

Quaternion Quaternion::Subtract(Quaternion quaternion) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion{
		current.w - quaternion.w,
		current.x - quaternion.x,
		current.y - quaternion.y,
		current.z - quaternion.z
	};
}

Quaternion Quaternion::Multiply(Quaternion quaternion) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		current.w * quaternion.w - current.x * quaternion.x - current.y * quaternion.y - current.z * quaternion.z,
		current.w * quaternion.x + current.x * quaternion.w + current.y * quaternion.z - current.z * quaternion.y,
		current.w * quaternion.y - current.x * quaternion.z + current.y * quaternion.w + current.z * quaternion.x,
		current.w * quaternion.z + current.x * quaternion.y - current.y * quaternion.x + current.z * quaternion.w
	};
}

Quaternion Quaternion::Multiply(float scalar) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion{
		current.w * scalar,
		current.x * scalar,
		current.y * scalar,
		current.z * scalar
	};
}

Quaternion operator  *(float scalar, Quaternion q) {
	Quaternion quaternion = Quaternion(q.w, q.x, q.y, q.z);

	return quaternion.Multiply(scalar);
}

Quaternion operator  *(Quaternion q, float scalar) {
	Quaternion quaternion = Quaternion(q.w, q.x, q.y, q.z);

	return quaternion.Multiply(scalar);
}

Quaternion Quaternion::Divide(Quaternion quaternion) {
	float scale = quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z;
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		(current.w * quaternion.w + current.x * quaternion.x + current.y * quaternion.y + current.z * quaternion.z) / scale,
		(-current.w * quaternion.x + current.x * quaternion.w + current.y * quaternion.z - current.z * quaternion.y) / scale,
		(-current.w * quaternion.y - current.x * quaternion.z + current.y * quaternion.w + current.z * quaternion.x) / scale,
		(-current.w * quaternion.z + current.x * quaternion.y - current.y * quaternion.x + current.z * quaternion.w) / scale
	};
}

Quaternion Quaternion::Divide(float scalar) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		current.w / scalar,
		current.x / scalar,
		current.y / scalar,
		current.z / scalar
	};
}

Quaternion Quaternion::Power(Quaternion exponent) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		pow(current.w, exponent.w),
		pow(current.x, exponent.x),
		pow(current.y, exponent.y),
		pow(current.z, exponent.z)
	};
}

Quaternion Quaternion::Power(float exponent) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		pow(current.w, exponent),
		pow(current.x, exponent),
		pow(current.y, exponent),
		pow(current.z, exponent)
	};
}

Quaternion Quaternion::Permutate(Vector3 permutation) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);
	float perm[3];

	perm[(int)permutation.x] = current.x;
	perm[(int)permutation.y] = current.y;
	perm[(int)permutation.z] = current.z;

	current.x = perm[0];
	current.y = perm[1];
	current.z = perm[2];

	return current;
}

Quaternion Quaternion::Absolute() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		abs(current.w),
		abs(current.x),
		abs(current.y),
		abs(current.z)
	};
}

Quaternion Quaternion::AdditiveInverse() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		-current.w,
		-current.x,
		-current.y,
		-current.z
	};
}

Quaternion Quaternion::MultiplicativeInverse() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return current.Conjugate().Multiply(1.0 / current.Normal());

}

Quaternion Quaternion::Conjugate() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return Quaternion
	{
		 current.w,
		-current.x,
		-current.y,
		-current.z
	};
}

Quaternion Quaternion::UnitQuaternion() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	float n = current.Magnitude();

	current.w = current.w / n;
	current.x = current.x / n;
	current.y = current.y / n;
	current.z = current.z / n;

	return current;
}

float Quaternion::Magnitude() {
	return sqrt(Normal());
}

float Quaternion::DotProduct(Quaternion q) {
	return (w * q.w) + (x * q.x) + (y * q.y) + (z * q.z);
}

float Quaternion::Normal() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);


	return pow(current.w, 2.0) + pow(current.x, 2.0) + pow(current.y, 2.0) + pow(current.z, 2.0);
}

bool Quaternion::IsNonZero() {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return current.w != 0 && current.x != 0 && current.y != 0 && current.z != 0;
}

bool Quaternion::IsEqual(Quaternion quaternion) {
	Quaternion current = Quaternion(this->w, this->x, this->y, this->z);

	return
		current.w == quaternion.w &&
		current.x == quaternion.x &&
		current.y == quaternion.y &&
		current.z == quaternion.z;
}