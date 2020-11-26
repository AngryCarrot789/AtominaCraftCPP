#pragma once
#include <vector>
#include <memory>
#include "../Rendering/Camera.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix4.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
#include "../Game/Debugging/DebugDraw.h"

//Forward declarations
class PhysicalGameObject;
class Mesh;
class Texture;
class Shader;

class GameObject {
public:
	GameObject();
	virtual ~GameObject() {}

	virtual void Reset();
	virtual void Draw(const Camera& cam, uint32_t curFBO);
	virtual void Update() {};
	virtual void OnHit(GameObject& other, Vector3& push) {};

	//Casts
	virtual PhysicalGameObject* AsPhysicalGameObject() { return nullptr; }
	const PhysicalGameObject* AsPhysicalGameObject() const { return const_cast<GameObject*>(this)->AsPhysicalGameObject(); }

	void DebugDraw(const Camera& cam);

	// Returns a matrix that can be used to determind the location 
	// of the vertices of a specific object in world space
	virtual Matrix4 LocalToWorld();
	virtual Matrix4 WorldToLocal();

	Vector3 Forward() const;

	Point pos;
	Euler euler;
	Size scale;

	virtual void SetPosition(Point _pos) {
		pos = _pos;
	}
	virtual void SetPosition(float x, float y, float z) {
		pos = Vector3(x, y, z);
	}
	virtual void SetScale(Size size) {
		scale = size;
	}
	virtual void SetScale(float x, float y, float z) {
		scale = Vector3(x, y, z);
	}

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Shader> shader;

	bool doesDebugDraw;
};