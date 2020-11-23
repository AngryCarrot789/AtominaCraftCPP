#pragma once
#include "../GameObject.h"
#include "../../Resources.h"

class Statue : public GameObject {
public:
  Statue(const char* model) {
    mesh = AquireMesh(model);
    shader = AquireShader("texture");
    texture = AquireTexture("gold.bmp");
  }
  virtual ~Statue() {}
};
