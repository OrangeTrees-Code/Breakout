#pragma once

#include "InitKey.hpp"
#include "Shader.tpp"
#include "Texture2D.hpp"
#include <memory>
class SpriteRenderer {

public:
  static SpriteRenderer &Instance();
  static void Init(InitKey, Shader &shader_ptr);
  SpriteRenderer(const SpriteRenderer &other) = delete;
  SpriteRenderer &operator=(const SpriteRenderer &other) = delete;
  ~SpriteRenderer() = default;

  void DrawSprite(Texture2D &texture2d, glm::vec2 position, glm::vec2 size = glm::vec2(10.f, 10.f), float rotate = 0.f, glm::vec3 color = glm::vec3(1.f));
  void SetShader(Shader &shader);
  Shader &GetShader() { return *shader_ptr_; }

private:
  void initRenderData();

private:
  static std::unique_ptr<SpriteRenderer> instance_;

  SpriteRenderer(Shader &shader_ptr) : shader_ptr_(&shader_ptr), vao_(0) { initRenderData(); }
  Shader *shader_ptr_;
  GLuint vao_;
};
