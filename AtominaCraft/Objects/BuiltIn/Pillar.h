#pragma once
#include "../GameObject.h"
#include "../../Resources.h"

class Pillar : public GameObject {
public:
  Pillar() {
    mesh = AquireMesh("pillar.obj");
    shader = AquireShader("texture");
    texture = AquireTexture("white.bmp");
    scale = Vector3(0.1f);
  }
  virtual ~Pillar() {}
};
