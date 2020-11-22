#pragma once
#include "../Object.h"
#include "../../Resources.h"

class Tunnel : public Object {
public:
  enum Type {
    NORMAL = 0,
    SCALE = 1,
    SLOPE = 2,
  };

  Tunnel(Type type) : type(type) {
    if (type == SCALE) {
      mesh = AquireMesh("tunnel_scale.obj");
    } else if (type == SLOPE) {
      mesh = AquireMesh("tunnel_slope.obj");
    } else {
        //mesh = AquireMesh("tunnel.obj");
        mesh = AquireMesh("tunnel.obj");
    }
    shader = AquireShader("texture");
    texture = AquireTexture("electromagnet.bmp");
    //texture = AquireTexture("checker_gray.bmp");
  }
  virtual ~Tunnel() {}

private:
  Type type;
};
