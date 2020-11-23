#pragma once
#include "../GameObject.h"
#include "../../Resources.h"
#include "../../Math/Vector3.h"
class Floorplan : public GameObject {
public:
  Floorplan() {
    mesh = AquireMesh("floorplan.obj");
    shader = AquireShader("texture_array");
    texture = AquireTexture("floorplan_textures.bmp", 4, 4);
    scale = Vector3(0.1524f); //6-inches to meters
  }
  virtual ~Floorplan() {}
};
