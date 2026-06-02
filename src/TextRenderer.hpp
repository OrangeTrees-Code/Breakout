/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "InitKey.hpp"
#include "ResourceManager.hpp"
#include "Shader.tpp"

// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer {
public:
  static TextRenderer &Instance();
  static void Init(InitKey, unsigned int width, unsigned int height, Shader &sdr);
  TextRenderer(const TextRenderer &other) = delete;
  TextRenderer &operator=(const TextRenderer &other) = delete;

  // Renders a string of text using the precompiled list of characters
  void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
  Shader &GetShader() { return *shader_; }
  void SetShader(Shader &sdr) { shader_ = &sdr; }
  Font &GetFont() { return *font_; }
  void SetFont(Font &font) { font_ = &font; }

private:
  // Constructor
  TextRenderer(GLuint width, GLuint height);

private:
  static std::unique_ptr<TextRenderer> instance_;
  // Render state
  unsigned int vao_;
  unsigned int vbo_;
  // Holds a list of pre-compiled Characters

  // Shader used for text rendering
  Shader *shader_;
  Font *font_;
};
