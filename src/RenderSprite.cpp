#include "RenderSprite.hpp"
#include "Shader.tpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

SpriteRender SpriteRender::instance_;

void SpriteRender::SetShader(Shader &shader) {
  shader_ptr_ = &shader;
  if (!initialized_) {
    initRenderData();
    initialized_ = true;
  }
}

void SpriteRender::initRenderData() {
  GLfloat vertices[] = {
      // 位置     // 纹理
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 1.0f};

  GLuint vbo;
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void SpriteRender::DrawSprite(Texture2D &texture2d, glm::vec2 position, glm::vec2 scale, float rotateAngle, glm::vec3 color) {
  glm::mat4 model(1.0f);

  model = glm::translate(model, glm::vec3(position, 0.f));

  model = glm::translate(model, glm::vec3(0.5 * scale.x, 0.5 * scale.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotateAngle), glm::vec3(0.f, 0.f, 1.f));
  model = glm::translate(model, glm::vec3(-0.5 * scale.x, -0.5 * scale.y, 0.0f));

  model = glm::scale(model, glm::vec3(scale, 1.0f));

  this->shader_ptr_->Use();
  shader_ptr_->SetUniform("model", model);
  shader_ptr_->SetUniform("color", color);

  glActiveTexture(GL_TEXTURE0);
  texture2d.Bind();

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
