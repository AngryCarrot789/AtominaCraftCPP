#include "AxisAlignedBB.h"

float AxisAlignedBB::CalculateOffsetX(const AxisAlignedBB aabb, float a) {
	if (aabb.maxY > minY && aabb.minY < maxY) {
		if (aabb.maxZ > minZ && aabb.minZ < maxZ) {
			float b = 0;
			if (a > 0.0f && aabb.maxX <= minX) {
				b = minX - aabb.maxX;
				if (b < a) {
					a = b;
				}
			}
			if (a < 0.0f && aabb.minX >= maxX) {
				b = maxX - aabb.minX;
				if (b > a) {
					a = b;
				}
			}
			return a;
		}
		else return a;
	}
	else return a;
}

float AxisAlignedBB::CalculateOffsetY(const AxisAlignedBB aabb, float a) {
	if (aabb.maxX > minX && aabb.minX < maxX) {
		if (aabb.maxZ > minZ && aabb.minZ < maxZ) {
			float b = 0;
			if (a > 0.0f && aabb.maxY <= minY) {
				b = minY - aabb.maxY;
				if (b < a) {
					a = b;
				}
			}
			if (a < 0.0f && aabb.minY >= maxY) {
				b = maxY - aabb.minY;
				if (b > a) {
					a = b;
				}
			}
			return a;
		}
		else return a;
	}
	else return a;
}

float AxisAlignedBB::CalculateOffsetZ(const AxisAlignedBB aabb, float a) {
	if (aabb.maxX > minX && aabb.minX < maxX) {
		if (aabb.maxY > minY && aabb.minY < maxY) {
			float b = 0;
			if (a > 0.0f && aabb.maxZ <= minZ) {
				b = minZ - aabb.maxZ;
				if (b < a) {
					a = b;
				}
			}
			if (a < 0.0f && aabb.minZ >= maxZ) {
				b = maxZ - aabb.minZ;
				if (b > a) {
					a = b;
				}
			}
			return a;
		}
		else return a;
	}
	else return a;
}

bool AxisAlignedBB::AABBIntersects(AxisAlignedBB aabb) {
	return
		aabb.maxX > minX && aabb.minX < maxX&&
		aabb.maxY > minY && aabb.minY < maxY&&
		aabb.maxZ > minZ && aabb.minZ < maxZ;
}

bool AxisAlignedBB::VectorIntersects(Point p) {
	return
		p.x > minX && p.x < maxX&&
		p.y > minY && p.y < maxY&&
		p.z > minZ && p.z < maxZ;
}

bool AxisAlignedBB::XYIntersects(float x, float y) {
	return
		x >= minX && x < maxX&&
		y >= minY && x < maxY;
}

bool AxisAlignedBB::YZIntersects(float y, float z) {
	return
		y >= minY && y < maxY&&
		z >= minZ && z < maxZ;
}

bool AxisAlignedBB::ZXIntersects(float z, float x) {
	return
		z >= minZ && z < maxZ&&
		x >= minX && x < maxX;
}