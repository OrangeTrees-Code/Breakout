#pragma once

#include "DynamicGOBase.hpp"
#include "IManipulable.ipp"
class Paddle : public DynamicGOBase, public IManipulable {
public:
  Paddle() : DynamicGOBase() {};
  Paddle(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : DynamicGOBase(position, size, sprite, color, velocity) {};
  ~Paddle() = default;
  void Move(float deltaTime, int width, int height) override;
  void ManipulableHandle(std::array<bool, 1024> &Keys) override;
};
