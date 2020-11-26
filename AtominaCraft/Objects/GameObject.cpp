#include "GameObject.h"
#include "../Game/Debugging/DebugDraw.h"

GameObject::GameObject() :
    pos(0.0f),
    euler(0.0f),
    scale(1.0f) {
    doesDebugDraw = true;
}

void GameObject::Reset() {
    SetPosition(Vector3::Zero());
    SetScale(Vector3::Zero());
    euler.SetZero();
}

void GameObject::Draw(const Camera& cam, uint32_t curFBO) {
    if (shader && mesh) {
        const Matrix4 mv = WorldToLocal().Transposed();
        const Matrix4 mvp = cam.Matrix() * LocalToWorld();
        shader->Use();
        if (texture) {
            texture->Use();
        }
        shader->SetMVP(mvp.m, mv.m);
        mesh->Draw();
    }
}

Vector3 GameObject::Forward() const {
  return 
      -(Matrix4::RotatedZ(euler.z) * 
        Matrix4::RotatedX(euler.x) * 
        Matrix4::RotatedY(euler.y)
        ).ZAxis();
}

void GameObject::DebugDraw(const Camera& cam)
{
    if (doesDebugDraw) {
        DebugDrawCube(cam, pos, scale);
    }
}

Matrix4 GameObject::LocalToWorld() {
  return 
      Matrix4::Translated(pos) * 
      Matrix4::RotatedY(euler.y) * 
      Matrix4::RotatedX(euler.x) * 
      Matrix4::RotatedZ(euler.z) * 
      Matrix4::Scaled(scale);
}

Matrix4 GameObject::WorldToLocal() {
    return 
        Matrix4::Scaled(1.0f / scale) * 
        Matrix4::RotatedZ(-euler.z) * 
        Matrix4::RotatedX(-euler.x) * 
        Matrix4::RotatedY(-euler.y) * 
        Matrix4::Translated(-pos);
}