#pragma once
#include <vector>
#include <memory>
#include "../Rendering/Camera.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix4.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"

//Forward declarations
class Physical;
class Mesh;
class Texture;
class Shader;

class Object {
public:
	Object();
	virtual ~Object() {}

	virtual void Reset();
	virtual void Draw(const Camera& cam, uint32_t curFBO);
	virtual void Update() {};
	virtual void OnHit(Object& other, Vector3& push) {};

	//Casts
	virtual Physical* AsPhysical() { return nullptr; }
	const Physical* AsPhysical() const { return const_cast<Object*>(this)->AsPhysical(); }

	void DebugDraw(const Camera& cam);

	// Returns a matrix that can be used to determind the location 
	// of the vertices of a specific object in world space
	Matrix4 LocalToWorld() const;
	Matrix4 WorldToLocal() const;

	Vector3 Forward() const;

	Point pos;
	Euler euler;
	Size scale;

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Shader> shader;
};