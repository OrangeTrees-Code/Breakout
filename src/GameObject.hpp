#pragma once

#include "SpriteRenderer.hpp"
#include "Texture2D.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

enum class E_ObjectType {
  GameObject,
  DynamicGOBase,
  Ball,
  Brick,
  Paddle,
  PowerUp,
};
class GameObject {
public:
  glm::vec2 Position;
  glm::vec2 Size;
  glm::vec3 Color;
  float Rotation = 0.f;
  bool IsSolid = false;
  bool Destroyed = false;
  Texture2D *Sprite;
  E_ObjectType ObjType;

public:
  GameObject();
  GameObject(E_ObjectType type);
  GameObject(E_ObjectType type, const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f));
  virtual ~GameObject() = default;
  virtual std::unique_ptr<GameObject> Clone() const;
  virtual void Draw(SpriteRenderer &renderer);
};
