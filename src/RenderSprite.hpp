#pragma once

#include "Shader.tpp"
#include "Texture2D.hpp"
class SpriteRender {
public:
  static SpriteRender &Instance() { return instance_; }
  ~SpriteRender() = default;

  void DrawSprite(Texture2D &texture2d, glm::vec2 position, glm::vec2 size = glm::vec2(10.f, 10.f), float rotate = 0.f, glm::vec3 color = glm::vec3(1.f));
  void SetShader(Shader &shader);
  Shader &GetShader() { return *shader_ptr_; }

private:
  void initRenderData();

private:
  static SpriteRender instance_;

  SpriteRender() : shader_ptr_(nullptr), vao_(0), initialized_(false) {};
  SpriteRender(Shader &shader_ptr) : shader_ptr_(&shader_ptr), vao_(0), initialized_(false) {};
  Shader *shader_ptr_;
  GLuint vao_;
  bool initialized_;
};
