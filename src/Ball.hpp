#pragma once

#include "DynamicGOBase.hpp"
#include "Event.hpp"
#include "GameObject.hpp"
#include "ICollision.ipp"
#include "Paddle.hpp"
class Ball : public DynamicGOBase, public IManipulable, public ICollision {

public:
  float Radius;
  bool IsStuck;
  Event<> OnOutOfBound;
  bool IsPassThrough = false;
  glm::vec3 EffectColor;

public:
  Ball() : DynamicGOBase(E_ObjectType::Ball) {};
  Ball(float radius, bool isStuck, const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : DynamicGOBase(E_ObjectType::Ball, position, size, sprite, color, velocity), Radius(radius), IsStuck(isStuck) {};

  void Move(float deltaTime, int width, int height) override;
  void ManipulableHandle(std::array<bool, 1024> &Keys, std::array<bool, 1024> &KeysProcessed) override;
  void SetLaunchPaddle(Paddle &paddle,bool isSyncOffset = false);

  CollisionInfo_Type CheckCollision(const GameObject &other) override;
  void OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) override;
  void Reset(const Paddle &paddle);

private:
  Paddle *launchPaddle_ = nullptr;
  glm::vec2 offsetToPaddle;

private:
  void resetOffsetWithPaddle_();
  void handleColliWithObjectX(const GameObject &other, BoundMaskType colliType);
  void handleColliWithObjectY(const GameObject &other, BoundMaskType colliType);
  void handleColliWithObject(const GameObject &other, BoundMaskType colliType);
  void handleColliWithPaddle(const GameObject &other, BoundMaskType colliType);
  void handleColliWithPaddleY(const GameObject &other, bool isUp);
  bool shouldUpdateOffset_ = false;
};
