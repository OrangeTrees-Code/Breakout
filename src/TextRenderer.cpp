/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "TextRenderer.hpp"
#include "ResourceManager.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/matrix_transform.hpp>

std::unique_ptr<TextRenderer> TextRenderer::instance_ = nullptr;

TextRenderer &TextRenderer::Instance() {
  if (!instance_) {
    throw std::runtime_error("TextRenderer has not been initialized");
  }
  return *instance_;
}
void TextRenderer::Init(InitKey, unsigned int width, unsigned int height, Shader &sdr) {
  instance_ = std::unique_ptr<TextRenderer>(new TextRenderer(width, height));
  instance_->SetShader(sdr);
}

TextRenderer::TextRenderer(GLuint width, GLuint height) {

  // Configure VAO/VBO for texture quads
  glGenVertexArrays(1, &this->vao_);
  glGenBuffers(1, &this->vbo_);
  glBindVertexArray(this->vao_);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
  // Activate corresponding render state
  this->shader_->Use();
  this->shader_->SetUniform("textColor", color);
  glActiveTexture(GL_TEXTURE3);
  glBindVertexArray(this->vao_);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Font::Character ch = font_->Characters[*c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y + (this->font_->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] = {
        {xpos, ypos + h, 0.0, 1.0},
        {xpos + w, ypos, 1.0, 0.0},
        {xpos, ypos, 0.0, 0.0},

        {xpos, ypos + h, 0.0, 1.0},
        {xpos + w, ypos + h, 1.0, 1.0},
        {xpos + w, ypos, 1.0, 0.0}};
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.Texture->Get_ID());
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph
    x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
