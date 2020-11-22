#pragma once
#include <memory>
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

std::shared_ptr<Mesh> AquireMesh(const char* name);
std::shared_ptr<Shader> AquireShader(const char* name);
std::shared_ptr<Texture> AquireTexture(const char* name, int rows=1, int cols=1);
