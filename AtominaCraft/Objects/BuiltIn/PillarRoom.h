#pragma once
#include "../GameObject.h"
#include "../../Resources.h"
#include "../../GameHeader.h"

class PillarRoom : public GameObject {
public:
  PillarRoom() {
    mesh = AquireMesh("pillar_room.obj");
    shader = AquireShader("texture");
    texture = AquireTexture("three_room.bmp");
    scale = Vector3(1.1f);
  }

  virtual ~PillarRoom() {}
};