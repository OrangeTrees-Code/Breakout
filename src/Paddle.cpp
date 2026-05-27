#include "Paddle.hpp"
void Paddle::Move(float deltaTime, int width, int height) {
  if (this->Velocity != glm::vec2(0.f, 0.f)) {
    this->Position += this->Velocity * deltaTime;
    if (BoundMaskType outType = isObjectOutOfBound(width, height); outType != (BoundMaskType)E_CrossBound::Nope) {
      if (outType & (BoundMaskType)E_CrossBound::Left) {
        this->Position.x = 0;
      }
      if (outType & (BoundMaskType)E_CrossBound::Right) {
        this->Position.x = width - this->Size.x;
      }
      if (outType & (BoundMaskType)E_CrossBound::Up) {
        this->Position.y = 0;
      }
      if (outType & (BoundMaskType)E_CrossBound::Down) {
        this->Position.y = height - this->Size.y;
      }
    }
  }
}

void Paddle::ManipulableHandle(std::array<bool, 1024> &Keys) {
  this->Velocity = glm::vec2(0.f);
  if (Keys[GLFW_KEY_A]) {
    this->Velocity.x = -500.f;
  }
  if (Keys[GLFW_KEY_D]) {
    this->Velocity.x = 500.f;
  }
  if (Keys[GLFW_KEY_W]) {
    this->Velocity.y = -500.f;
  }
  if (Keys[GLFW_KEY_S]) {
    this->Velocity.y = 500.f;
  }
}

void Paddle::Reset(int width, int height) {
  this->Position = glm::vec2((float)width / 2 - Size.x / 2, height - Size.y);
}