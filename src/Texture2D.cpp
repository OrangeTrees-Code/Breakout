/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include <glad/glad.h>
#include "Texture2D.hpp"

Texture2D::Texture2D()
    : width_(0), height_(0), internal_format_(GL_RGB), image_format_(GL_RGB), wrap_s_(GL_REPEAT), wrap_t_(GL_REPEAT), filter_min_(GL_LINEAR), filter_max_(GL_LINEAR) {
  glGenTextures(1, &this->id_);
}

Texture2D::Texture2D(Texture2D &&other) noexcept
    : id_(other.id_), width_(other.width_), height_(other.height_),
      internal_format_(other.internal_format_),
      image_format_(other.image_format_), wrap_s_(other.wrap_s_),
      wrap_t_(other.wrap_t_), filter_min_(other.filter_min_),
      filter_max_(other.filter_max_) {
  other.id_ = 0;
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
  if (this != &other) {
    release();
    id_ = other.id_;
    width_ = other.width_;
    height_ = other.height_;
    internal_format_ = other.internal_format_;
    image_format_ = other.image_format_;
    wrap_s_ = other.wrap_s_;
    wrap_t_ = other.wrap_t_;
    filter_min_ = other.filter_min_;
    filter_max_ = other.filter_max_;
    other.id_ = 0;
  }
  return *this;
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char *data) {
  this->width_ = width;
  this->height_ = height;
  // Create Texture
  glBindTexture(GL_TEXTURE_2D, this->id_);
  glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format_, width, height, 0, this->image_format_, GL_UNSIGNED_BYTE, data);
  // Set Texture wrap and filter modes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max_);
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
