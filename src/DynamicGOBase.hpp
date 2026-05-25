#pragma once

#include "GameObject.hpp"
#include "IMovable.ipp"
#include <glm/ext/vector_float2.hpp>
enum class E_OutOfBound : unsigned int {
  Nope = 0u,
  Left = 1u,
  Right = 2u,
  Down = 4u,
  Up = 8u,
};

class DynamicGOBase : public IMovable, public GameObject {
public:
  using OutOfBoundMask = unsigned int;

public:
  glm::vec2 Velocity;

public:
  DynamicGOBase() : GameObject(), Velocity(glm::vec2(0.f, 0.f)) {};
  DynamicGOBase(const glm::vec2 &velocity) : Velocity(velocity) {};
  DynamicGOBase(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : GameObject(position, size, sprite, color), Velocity(velocity) {};

protected:
  OutOfBoundMask isObjectOutOfBound(int width, int height);
};
