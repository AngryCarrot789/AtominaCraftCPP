#pragma once
#include "../GameObject.h"
#include "../../Resources.h"
#include "../../GameHeader.h"

class House : public GameObject {
public:
  House(const char* tex) {
    mesh = AquireMesh("square_rooms.obj");
    shader = AquireShader("texture");
    texture = AquireTexture(tex);
    scale = Vector3(1.0f, 3.0f, 1.0f);
  }
  virtual ~House() {}
};
