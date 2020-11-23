#pragma once
#include "../Math/Vector3.h"
#include <math.h>
#include "../GameHeader.h"
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

	float CalculateOffsetX(const AxisAlignedBB aabb, float a);
	float CalculateOffsetY(const AxisAlignedBB aabb, float a);
	float CalculateOffsetZ(const AxisAlignedBB aabb, float a);

	bool IsAABBIntersectingAABB(AxisAlignedBB aabb);
	bool IsVectorIntersectingAABB(Point p);

	Vector3 GetIntersection(AxisAlignedBB aabb) {
		return Vector3(
			GetIntersectionAmountX(aabb),
			GetIntersectionAmountY(aabb),
			GetIntersectionAmountZ(aabb));
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

	bool XYIntersects(float x, float y);
	bool YZIntersects(float y, float z);
	bool ZXIntersects(float z, float x);

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

	float GetIntersectionAmountX(AxisAlignedBB aabb) {
		bool maxOverlapsAABBMin = MaxOverlapsAABBMinX(aabb);
		bool minOverlapsAABBMax = MinOverlapsAABBMaxX(aabb);
		//if (maxOverlapsAABBMin && minOverlapsAABBMax) { return 0; }
		if (maxOverlapsAABBMin) { return maxX - aabb.minX; }
		else if (minOverlapsAABBMax) { return aabb.maxX - minX; }
		else { return 0; }
	}
	float GetIntersectionAmountY(AxisAlignedBB aabb) {
		bool maxOverlapsAABBMin = MaxOverlapsAABBMinY(aabb);
		bool minOverlapsAABBMax = MinOverlapsAABBMaxY(aabb);
		//if (maxOverlapsAABBMin && minOverlapsAABBMax) { return 0; }
		if (maxOverlapsAABBMin) { return maxY - aabb.minY; }
		else if (minOverlapsAABBMax) { return aabb.maxY - minY; }
		else { return 0; }
	}
	float GetIntersectionAmountZ(AxisAlignedBB aabb) {
		bool maxOverlapsAABBMin = MaxOverlapsAABBMinZ(aabb);
		bool minOverlapsAABBMax = MinOverlapsAABBMaxZ(aabb);
		//if (maxOverlapsAABBMin && minOverlapsAABBMax) { return 0; }
		if (maxOverlapsAABBMin) { return maxZ - aabb.minZ; }
		else if (minOverlapsAABBMax) { return aabb.maxZ - minZ; }
		else { return 0; }
	}

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