#pragma once

#include "GameObject.hpp"
#include "IMovable.ipp"
#include <glm/ext/vector_float2.hpp>
enum class E_CrossBound : unsigned int {
  Nope = 0u,
  Left = 1u,
  Right = 2u,
  Down = 4u,
  Up = 8u,
};

class DynamicGOBase : public IMovable, public GameObject {
public:
  using BoundMaskType = unsigned int;

public:
  glm::vec2 Velocity;

public:
  DynamicGOBase() : GameObject(E_ObjectType::DynamicGOBase), Velocity(glm::vec2(0.f, 0.f)) {};
  DynamicGOBase(const glm::vec2 &velocity) : GameObject(E_ObjectType::DynamicGOBase), Velocity(velocity) {};
  DynamicGOBase(E_ObjectType type) : GameObject(type), Velocity(glm::vec2(0.f, 0.f)) {};
  DynamicGOBase(E_ObjectType type, const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : GameObject(type, position, size, sprite, color), Velocity(velocity) {};

protected:
  BoundMaskType isObjectOutOfBound(int width, int height);
  void Move(float deltaTime, int width, int height) override {}
};
