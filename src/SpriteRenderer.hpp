#pragma once

#include "Shader.tpp"
#include "Texture2D.hpp"
class SpriteRenderer {
public:
  static SpriteRenderer &Instance() { return instance_; }
  ~SpriteRenderer() = default;

  void DrawSprite(Texture2D &texture2d, glm::vec2 position, glm::vec2 size = glm::vec2(10.f, 10.f), float rotate = 0.f, glm::vec3 color = glm::vec3(1.f));
  void SetShader(Shader &shader);
  Shader &GetShader() { return *shader_ptr_; }

private:
  void initRenderData();

private:
  static SpriteRenderer instance_;

  SpriteRenderer() : shader_ptr_(nullptr), vao_(0), initialized_(false) {};
  SpriteRenderer(Shader &shader_ptr) : shader_ptr_(&shader_ptr), vao_(0), initialized_(false) {};
  Shader *shader_ptr_;
  GLuint vao_;
  bool initialized_;
};
