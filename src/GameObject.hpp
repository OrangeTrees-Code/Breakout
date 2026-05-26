#pragma once

#include "SpriteRenderer.hpp"
#include "Texture2D.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
class GameObject {
public:
  glm::vec2 Position;
  glm::vec2 Size;
  glm::vec3 Color;
  float Rotation = 0.f;
  bool IsSolid = true;
  bool Destroyed = false;
  Texture2D *Sprite;

public:
  GameObject();
  GameObject(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f));
  virtual void Draw(SpriteRenderer &renderer);
  virtual bool CheckCollision(const GameObject &other);
  virtual void OnCollision(const GameObject &other);
};