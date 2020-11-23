#pragma once
#include "../Math/Vector3.h"
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

	// Sets the minimums and maximums based on the center coordinates and scale
	void SetFromCenter(Point pos, Size scale) {
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

	void SetAABB(AxisAlignedBB aabb) {
		SetBounds(aabb.minX, aabb.minY, aabb.minZ, aabb.maxX, aabb.maxY, aabb.maxZ);
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

	float CalculateOffsetX(const AxisAlignedBB aabb, float a);
	float CalculateOffsetY(const AxisAlignedBB aabb, float a);
	float CalculateOffsetZ(const AxisAlignedBB aabb, float a);

	bool AABBIntersects(AxisAlignedBB aabb);
	bool VectorIntersects(Point p);

	float GetAverageEdgeLength() {
		float x = maxX - minX;
		float y = maxY - minY;
		float z = maxZ - minZ;
		return (x + y + z) / 3.0f;
	}

	bool XYIntersects(float x, float y);
	bool YZIntersects(float y, float z);
	bool ZXIntersects(float z, float x);

	Point GetMinimum() { return Point(minX, minY, minZ); }
	Point GetMaximum() { return Point(maxX, maxY, maxZ); }

	float GetSizeX() { return IsDirectionFlippedX() ? minX - maxX : maxX - minX; }
	float GetSizeY() { return IsDirectionFlippedY() ? minY - maxY : maxY - minY; }
	float GetSizeZ() { return IsDirectionFlippedZ() ? minZ - maxZ : maxZ - minZ; }
	Size GetSize() { return Size(GetSizeX(), GetSizeY(), GetSizeZ()); }

	static AxisAlignedBB CreateAABBFromCenter(Point pos, Size scale) {
		AxisAlignedBB aabb;
		aabb.SetFromCenter(pos, scale);
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