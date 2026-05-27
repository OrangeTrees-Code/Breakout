#pragma once

#include "GameObject.hpp"
#include "ICollision.ipp"
class Brick : public GameObject, public ICollision {
public:
  Brick() : GameObject(E_ObjectType::Brick) {};
  Brick(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f)) : GameObject(E_ObjectType::Brick,position, size, sprite, color) {};

  void OnCollision(const GameObject &other, CollisionInfo_Type &collisionInfo) override;
  void Reset();
};