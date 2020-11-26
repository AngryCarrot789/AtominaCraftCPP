#pragma once
#include "../Math/Vector3.h"
#include <math.h>
#include "../GameHeader.h"
#include <iostream>
class AxisAlignedBB {
public:
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	AxisAlignedBB() : minX(0), minY(0), minZ(0), maxX(0), maxY(0), maxZ(0) {}

	AxisAlignedBB(float miX, float miY, float miZ, float maX, float maY, float maZ) {
		minX = miX; minY = miY; minZ = miZ;
		maxX = maX; maxY = maY; maxZ = maZ;
	}

	AxisAlignedBB(Point min, Point max) {
		minX = min.x; minY = min.y; minZ = min.z;
		maxX = max.x; maxY = max.y; maxZ = max.z;
	}

	void SetBounds(float miX, float miY, float miZ, float maX, float maY, float maZ) {
		minX = miX; minY = miY; minZ = miZ;
		maxX = maX; maxY = maY; maxZ = maZ;
	}

	void SetAABB(AxisAlignedBB aabb) {
		SetBounds(aabb.minX, aabb.minY, aabb.minZ, aabb.maxX, aabb.maxY, aabb.maxZ);
	}

	// Sets the minimums and maximums based on the center coordinates and scale
	void SetPositionFromCenter(Point pos, Size scale) {
		SetBounds(
			pos.x - (scale.x / 2),
			pos.y - (scale.y / 2),
			pos.z - (scale.z / 2),
			pos.x + (scale.x / 2),
			pos.y + (scale.y / 2),
			pos.z + (scale.z / 2)
		);
	}

	// Gets the center of the AABB based on its minimums and maximums
	Point GetCenter() {
		float sX = GetSizeX();
		float sY = GetSizeY();
		float sZ = GetSizeZ();

		return Point(
			minX + (sX / 2),
			minY + (sY / 2),
			minZ + (sZ / 2)
		);
	}

	AxisAlignedBB Copy() {
		return AxisAlignedBB(minX, minY, minZ, maxX, maxY, maxZ);
	}

	void Expand(float x, float y, float z) {
		minX -= x; minY -= y; minZ -= z;
		maxX += x; maxY += y; maxZ += z;
	}

	void Offset(float x, float y, float z) {
		minX += x; minY += y; minZ += z;
		maxX += x; maxY += y; maxZ += z;
	}

	float CalculateOffsetX(const AxisAlignedBB aabb) {
		float a = 0;
		if (aabb.maxY > minY && aabb.minY < maxY) {
			if (aabb.maxZ > minZ && aabb.minZ < maxZ) {
				float b = 0;
				if (aabb.maxX <= minX) {
					b = minX - aabb.maxX;
					if (b < 0) {
						a = b;
					}
				}
				if (aabb.minX >= maxX) {
					b = maxX - aabb.minX;
					if (b > 0) {
						a = b;
					}
				}
				return a;
			}
			else return a;
		}
		else return a;
	}

	float CalculateOffsetY(const AxisAlignedBB aabb) {
		float a = 0;
		if (aabb.maxX > minX && aabb.minX < maxX) {
			if (aabb.maxZ > minZ && aabb.minZ < maxZ) {
				float b = 0;
				if (aabb.maxY <= minY) {
					b = minY - aabb.maxY;
					if (b < 0) {
						a = b;
					}
				}
				if (aabb.minY >= maxY) {
					b = maxY - aabb.minY;
					if (b > 0) {
						a = b;
					}
				}
				return a;
			}
			else return a;
		}
		else return a;
	}

	float CalculateOffsetZ(const AxisAlignedBB aabb) {
		float a = 0;
		if (aabb.maxX > minX && aabb.minX < maxX) {
			if (aabb.maxY > minY && aabb.minY < maxY) {
				float b = 0;
				if (aabb.maxZ <= minZ) {
					b = minZ - aabb.maxZ;
					if (b < 0) {
						a = b;
					}
				}
				if (aabb.minZ >= maxZ) {
					b = maxZ - aabb.minZ;
					if (b > 0) {
						a = b;
					}
				}
				return a;
			}
			else return a;
		}
		else return a;
	}

	bool IsAABBIntersectingAABB(AxisAlignedBB aabb) {
		bool x = IntersectsAABBX(aabb);
		bool y = IntersectsAABBY(aabb);
		bool z = IntersectsAABBZ(aabb);
		bool isIntersect = x && y && z;
		return isIntersect;
	}

	bool IsVectorIntersectingAABB(Point p) {
		return
			(p.x > minX && p.x < maxX) &&
			(p.y > minY && p.y < maxY) &&
			(p.z > minZ && p.z < maxZ);
	}

	Vector3 GetIntersection(AxisAlignedBB aabb) {
		return
			Vector3(
				GetIntersectionAmountX(aabb),
				GetIntersectionAmountY(aabb),
				GetIntersectionAmountZ(aabb)
			);
	}

	bool IsAABBVecIntersecting(Vector3 intersection) {
		return intersection.x != 0.0f && intersection.y != 0.0f && intersection.z != 0.0f;
	}


	float GetAverageEdgeLength() {
		float x = maxX - minX;
		float y = maxY - minY;
		float z = maxZ - minZ;
		return (x + y + z) / 3.0f;
	}

	// returns true if the X axis intersects a given AABB's X axis
	bool IntersectsAABBX(AxisAlignedBB aabb) {
		return aabb.maxX > minX && aabb.minX < maxX;
	}

	// returns true if the Y axis intersects a given AABB's Y axis
	bool IntersectsAABBY(AxisAlignedBB aabb) {
		return aabb.maxY > minY && aabb.minY < maxY;
	}

	// returns true if the Z axis intersects a given AABB's Z axis
	bool IntersectsAABBZ(AxisAlignedBB aabb) {
		return aabb.maxZ > minZ && aabb.minZ < maxZ;
	}

	// Returns how much the X axis intersects the given AABB's X axis
	float GetIntersectionAmountX(AxisAlignedBB aabb) { 
		if (IntersectsAABBY(aabb) && IntersectsAABBZ(aabb)) {
			float val = IsBehindAABBX(aabb) ? maxX - aabb.minX : aabb.maxX - minX;
			//return val > 0 ? val : 0;
			return val;
		}
		return 0;
	}

	// Returns how much the Y axis intersects the given AABB's Y axis
	float GetIntersectionAmountY(AxisAlignedBB aabb) {
		if (IntersectsAABBX(aabb) && IntersectsAABBZ(aabb)) {
			float val = IsBehindAABBY(aabb) ? maxY - aabb.minY : aabb.maxY - minY;
			//return val > 0 ? val : 0;
			return val;
		}
		return 0;
	}

	// Returns how much the Z axis intersects the given AABB's Z axis
	float GetIntersectionAmountZ(AxisAlignedBB aabb) {
		if (IntersectsAABBX(aabb) && IntersectsAABBY(aabb)) {
			float val = IsBehindAABBZ(aabb) ? maxZ - aabb.minZ : aabb.maxZ - minZ;
			//return val > 0 ? val : 0;
			return val;
		}
		return 0;
	}

	Point GetMinimum() { return Point(minX, minY, minZ); }
	Point GetMaximum() { return Point(maxX, maxY, maxZ); }

	float GetSizeX() { return IsDirectionFlippedX() ? minX - maxX : maxX - minX; }
	float GetSizeY() { return IsDirectionFlippedY() ? minY - maxY : maxY - minY; }
	float GetSizeZ() { return IsDirectionFlippedZ() ? minZ - maxZ : maxZ - minZ; }
	Size GetSize() { return Size(GetSizeX(), GetSizeY(), GetSizeZ()); }

	bool MaxOverlapsAABBMinX(AxisAlignedBB aabb) { return maxX > aabb.minX; }
	bool MinOverlapsAABBMaxX(AxisAlignedBB aabb) { return minX < aabb.maxX; }
	bool MaxOverlapsAABBMinY(AxisAlignedBB aabb) { return maxY > aabb.minY; }
	bool MinOverlapsAABBMaxY(AxisAlignedBB aabb) { return minY < aabb.maxY; }
	bool MaxOverlapsAABBMinZ(AxisAlignedBB aabb) { return maxZ > aabb.minZ; }
	bool MinOverlapsAABBMaxZ(AxisAlignedBB aabb) { return minZ < aabb.maxZ; }

	bool IsBehindAABBX(AxisAlignedBB aabb) { return maxX < aabb.maxX && minX < aabb.minX; }
	bool IsBehindAABBY(AxisAlignedBB aabb) { return maxY < aabb.maxY && minY < aabb.minY; }
	bool IsBehindAABBZ(AxisAlignedBB aabb) { return maxZ < aabb.maxZ && minZ < aabb.minZ; }
	

	static AxisAlignedBB CreateAABBFromCenter(Point pos, Size scale) {
		AxisAlignedBB aabb;
		aabb.SetPositionFromCenter(pos, scale);
		return aabb;
	}

	static AxisAlignedBB CreateAABB(Point min, Point max) {
		AxisAlignedBB aabb = AxisAlignedBB(min, max);
		return aabb;
	}

private:
	bool IsDirectionFlippedX() { return minX > maxX; }
	bool IsDirectionFlippedY() { return minY > maxY; }
	bool IsDirectionFlippedZ() { return minZ > maxZ; }
};