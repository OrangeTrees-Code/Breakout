/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include "Texture2D.hpp"
#include <glad/glad.h>

Texture2D::Texture2D()
    : Width(0), Height(0), Internal_format(GL_RGB), Image_format(GL_RGB), Wrap_s(GL_REPEAT), Wrap_t(GL_REPEAT), Filter_min(GL_LINEAR), Filter_max(GL_LINEAR) {
  glGenTextures(1, &this->id_);
}

Texture2D::Texture2D(Texture2D &&other) noexcept
    : id_(other.id_), Width(other.Width), Height(other.Height),
      Internal_format(other.Internal_format),
      Image_format(other.Image_format), Wrap_s(other.Wrap_s),
      Wrap_t(other.Wrap_t), Filter_min(other.Filter_min),
      Filter_max(other.Filter_max) {
  other.id_ = 0;
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
  if (this != &other) {
    release();
    id_ = other.id_;
    Width = other.Width;
    Height = other.Height;
    Internal_format = other.Internal_format;
    Image_format = other.Image_format;
    Wrap_s = other.Wrap_s;
    Wrap_t = other.Wrap_t;
    Filter_min = other.Filter_min;
    Filter_max = other.Filter_max;
    other.id_ = 0;
  }
  return *this;
}

void Texture2D::generate(GLuint width, GLuint height, unsigned char *data) {
  this->Width = width;
  this->Height = height;
  // Create Texture
  glBindTexture(GL_TEXTURE_2D, this->id_);
  glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_format, width, height, 0, this->Image_format, GL_UNSIGNED_BYTE, data);
  // Set Texture wrap and filter modes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_max);
  // Unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
  glBindTexture(GL_TEXTURE_2D, this->id_);
}

void Texture2D::release() {
  if (id_ != 0) {
    glDeleteTextures(1, &id_);
  }
  id_ = 0;
}
