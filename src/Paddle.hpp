#pragma once

#include "DynamicGOBase.hpp"
#include "ICollision.ipp"
#include "IManipulable.ipp"
class Paddle : public DynamicGOBase, public IManipulable, public ICollision {
public:
  bool IsSticky = false;
  glm::vec3 EffectColor;
  float MaxLength;

public:
  Paddle() : DynamicGOBase(E_ObjectType::Paddle) {};
  Paddle(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : DynamicGOBase(E_ObjectType::Paddle, position, size, sprite, color, velocity) {};
  ~Paddle() = default;
  void Move(float deltaTime, int width, int height) override;
  void ManipulableHandle(std::array<bool, 1024> &Keys) override;
  void Reset(int width, int height);
  CollisionInfo_Type CheckCollision(const GameObject &other) override;
  void OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) override;
  void Draw(SpriteRenderer &renderer) override;
};
