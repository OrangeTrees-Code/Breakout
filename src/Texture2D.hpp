/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once

#include "IResource.ipp"
#include <glad/glad.h>

class ResourceManager;
// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D : IResource {
  friend class ResourceManager;

public:
  // Constructor (sets default texture modes)
  // Generates texture from image data
  void Generate(GLuint width, GLuint height, unsigned char *data);
  // Binds the texture as the current active GL_TEXTURE_2D texture object
  void Bind() const;
  void release() override;
  Texture2D(Texture2D &&other) noexcept;
  Texture2D &operator=(Texture2D &&other) noexcept;
  Texture2D(const Texture2D &) = delete;
  Texture2D &operator=(const Texture2D &) = delete;
  ~Texture2D() { release(); }

private:
  // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
  GLuint id_;
  // Texture image dimensions
  GLuint width_, height_; // Width and height of loaded image in pixels
  // Texture Format
  GLuint internal_format_; // Format of texture object
  GLuint image_format_;    // Format of loaded image
  // Texture configuration
  GLuint wrap_s_;     // Wrapping mode on S axis
  GLuint wrap_t_;     // Wrapping mode on T axis
  GLuint filter_min_; // Filtering mode if texture pixels < screen pixels
  GLuint filter_max_; // Filtering mode if texture pixels > screen pixels
private:
  Texture2D();
};
