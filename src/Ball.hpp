#pragma once

#include "DynamicGOBase.hpp"
#include "GameObject.hpp"
#include "ICollision.ipp"
#include "Paddle.hpp"
#include "Event.hpp"
class Ball : public DynamicGOBase, public IManipulable, public ICollision {

public:
  float Radius;
  bool IsStuck;
  Event<> OnOutOfBound;

public:
  Ball() : DynamicGOBase(E_ObjectType::Ball) {};
  Ball(float radius, bool isStuck, const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : DynamicGOBase(E_ObjectType::Ball, position, size, sprite, color, velocity), Radius(radius), IsStuck(isStuck) {};

  void Move(float deltaTime, int width, int height) override;
  void ManipulableHandle(std::array<bool, 1024> &Keys) override;
  void SetLaunchPaddle(Paddle &paddle) { launchPaddle_ = &paddle; }

  CollisionInfo_Type CheckCollision(const GameObject &other) override;
  void OnCollision(const GameObject &other, CollisionInfo_Type &collisionInfo) override;
  void Reset(const Paddle& paddle);

private:
  Paddle *launchPaddle_ = nullptr;

private:
  void handleColliWithPaddle(const GameObject &other, bool isUp);
};