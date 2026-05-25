#include "Ball.hpp"
#include <GLFW/glfw3.h>

void Ball::Move(float deltaTime, int width, int height) {
  if (this->IsStuck && this->launchPaddle_ != nullptr) {
    this->Position = launchPaddle_->Position + glm::vec2(launchPaddle_->Size.x / 2.f - Radius, -Radius * 2.f);
  } else if (this->Velocity != glm::vec2(0.f, 0.f)) {
    this->Position += this->Velocity * deltaTime;
    if (OutOfBoundMask outType = isObjectOutOfBound(width, height); outType != (OutOfBoundMask)E_OutOfBound::Nope) {
      if (outType & (OutOfBoundMask)E_OutOfBound::Left) {
        this->Velocity.x *= -1;
        this->Position.x = 0;
      }
      if (outType & (OutOfBoundMask)E_OutOfBound::Right) {
        this->Velocity.x *= -1;
        this->Position.x = width - this->Size.x;
      }
      if (outType & (OutOfBoundMask)E_OutOfBound::Up) {
        this->Velocity.y *= -1;
        this->Position.y = 0;
      }
    }
  }
}

void Ball::ManipulableHandle(std::array<bool, 1024> &Keys) {
  if (Keys[GLFW_KEY_SPACE]) {
    this->IsStuck = false;
  }
}