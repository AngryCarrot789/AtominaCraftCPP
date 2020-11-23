#pragma once
#include "Camera.h"
#include "GL/glew.h"
#include <vector>
#include <map>

class Mesh {
public:
  static const int NUM_VBOS = 3;

  Mesh(const char* fname);
  ~Mesh();

  void Draw();

private:
  void AddFace(
    const std::vector<float>& vert_palette, const std::vector<float>& uv_palette,
    uint32_t a, uint32_t at, uint32_t b, uint32_t bt, uint32_t c, uint32_t ct, bool is3DTex);

  GLuint vao;
  GLuint vbo[NUM_VBOS];

  std::vector<float> verts;
  std::vector<float> uvs;
  std::vector<float> normals;
};
