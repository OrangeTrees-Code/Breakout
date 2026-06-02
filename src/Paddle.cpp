#include "Paddle.hpp"
#include "DynamicGOBase.hpp"
#include "PowerUp.hpp"
#include <cmath>
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
  if (Keys[GLFW_KEY_SPACE]) {
    if (this->IsSticky) {
      this->IsSticky = false;
    }
  }
}

void Paddle::Reset(int width, int height) {
  this->Position = glm::vec2((float)width / 2 - Size.x / 2, height - Size.y);
  this->Size = glm::vec2(100.f, 20.f);
  this->IsSticky = false;
}

ICollision::CollisionInfo_Type Paddle::CheckCollision(const GameObject &other) {
  const bool collisionX = this->Position.x + this->Size.x >= other.Position.x &&
                          other.Position.x + other.Size.x >= this->Position.x;
  const bool collisionY = this->Position.y + this->Size.y >= other.Position.y &&
                          other.Position.y + other.Size.y >= this->Position.y;

  if (!collisionX || !collisionY) {
    return {false, (BoundMaskType)E_CrossBound::Nope};
  }

  BoundMaskType mask = (BoundMaskType)E_CrossBound::Nope;

  const glm::vec2 paddleCenter = this->Position + this->Size * 0.5f;
  const glm::vec2 otherCenter = other.Position + other.Size * 0.5f;
  const glm::vec2 distance = paddleCenter - otherCenter;
  const float overlapX = this->Size.x * 0.5f + other.Size.x * 0.5f - std::abs(distance.x);
  const float overlapY = this->Size.y * 0.5f + other.Size.y * 0.5f - std::abs(distance.y);

  if (overlapX < overlapY) {
    mask |= distance.x < 0.f ? (BoundMaskType)E_CrossBound::Left : (BoundMaskType)E_CrossBound::Right;
  } else {
    mask |= distance.y < 0.f ? (BoundMaskType)E_CrossBound::Up : (BoundMaskType)E_CrossBound::Down;
  }

  return {true, mask};
}
void Paddle::OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) {
  switch (other.ObjType) {
  case E_ObjectType::Brick: {
    auto colliType = std::get<1>(collisionInfo);
    if (colliType & (BoundMaskType)E_CrossBound::Left) {
      this->Position.x = other.Position.x - this->Size.x;
      this->Velocity.x *= -1;
    } else if (colliType & (BoundMaskType)E_CrossBound::Right) {
      this->Position.x = other.Position.x + other.Size.x;
      this->Velocity.x *= -1;
    }
    if (colliType & (BoundMaskType)E_CrossBound::Up) {
      this->Position.y = other.Position.y - this->Size.y;
      this->Velocity.y *= -1;
    } else if (colliType & (BoundMaskType)E_CrossBound::Down) {
      this->Position.y = other.Position.y + other.Size.y;
      this->Velocity.y *= -1;
    }
    break;
  }
  case E_ObjectType::PowerUp: {
    dynamic_cast<PowerUp &>(other).Active();
    break;
  }
  default:
    break;
  }
}

void Paddle::Draw(SpriteRenderer &renderer) {
  if (IsSticky) {
    renderer.DrawSprite(*(this->Sprite), this->Position, this->Size, this->Rotation, this->EffectColor);
  } else {
    renderer.DrawSprite(*(this->Sprite), this->Position, this->Size, this->Rotation, this->Color);
  }
}