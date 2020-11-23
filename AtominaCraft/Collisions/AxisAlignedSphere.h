#pragma once
#include "../Math/Vector3.h"
#include "AxisAlignedBB.h"
class AxisAlignedSphere {
public:
	AxisAlignedSphere() { center = Point::Zero(); radius = 0; }
	AxisAlignedSphere(Point pos, float rad = 0) {
		center = pos;
		radius = rad;
	}
	AxisAlignedSphere(float x, float y, float z, float rad = 0) {
		center = Point(x, y, z);
		radius = rad;
	}

	bool AASphererIntersects(AxisAlignedSphere s) {
		float dist =
			std::sqrtf(
				(center.x - s.center.x) * (center.x - s.center.x) +
				(center.y - s.center.y) * (center.y - s.center.y) +
				(center.z - s.center.z) * (center.z - s.center.z));
		return dist < (radius + s.radius);
	}

	bool SphererIntersectsAABB(AxisAlignedBB aabb) {
		float x = std::fmaxf(aabb.minX, std::fminf(center.x, aabb.maxX));
		float y = std::fmaxf(aabb.minY, std::fminf(center.y, aabb.maxY));
		float z = std::fmaxf(aabb.minZ, std::fminf(center.z, aabb.maxZ));
		float dist =
			std::sqrtf(
				(x - center.x) * (x - center.x) +
				(y - center.y) * (y - center.y) +
				(z - center.z) * (z - center.z));
		return dist < radius;
	}

	Point center;
	float radius;
};