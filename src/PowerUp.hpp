#pragma once

#include "Ball.hpp"
#include "DynamicGOBase.hpp"
enum class E_PowerUpType {
  Speed,
  Sticky,
  PassThrough,
  PadSizeIncrease,
  Confuse,
  Chaos,
};

class PowerUp : public DynamicGOBase, public ICollision {
public:
  E_PowerUpType Type;
  float Duration;
  bool Actived;

public:
  PowerUp() : Type(E_PowerUpType::Speed), Duration(0.f), Actived(false), DynamicGOBase(E_ObjectType::PowerUp) {}
  PowerUp(E_PowerUpType type, float duration, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f), bool isActive = false, const glm::vec2 &position = glm::vec3(0.f)) : Type(type), Duration(duration), Actived(isActive), DynamicGOBase(E_ObjectType::PowerUp, position, size, sprite, color, velocity) {}

  void Move(float deltaTime, int width, int height) override;
  void Update(float deltaTime);
  void Init(InitKey, std::function<Ball &(void)> GetBallFunc, std::function<Paddle &(void)> GetPaddleFunc);
  void Active();
  void Deactive();
  std::unique_ptr<GameObject> Clone() const override;
  void OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) override;

private:
  std::function<Ball &(void)> GetBall;
  std::function<Paddle &(void)> GetPaddle;
};
