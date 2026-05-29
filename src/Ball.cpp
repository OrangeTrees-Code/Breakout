#include "Ball.hpp"
#include "DynamicGOBase.hpp"
#include "GameObject.hpp"
#include "Paddle.hpp"
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

void Ball::Move(float deltaTime, int width, int height) {
  if (this->IsStuck && this->launchPaddle_ != nullptr) {
    this->Position = launchPaddle_->Position + glm::vec2(launchPaddle_->Size.x / 2.f - Radius, -Radius * 2.f - 0.01f);
  } else if (this->Velocity != glm::vec2(0.f, 0.f)) {
    this->Position += this->Velocity * deltaTime;
    if (BoundMaskType outType = isObjectOutOfBound(width, height); outType != (BoundMaskType)E_CrossBound::Nope) {
      if (outType & (BoundMaskType)E_CrossBound::Left) {
        this->Velocity.x *= -1;
        this->Position.x = 0;
      }
      if (outType & (BoundMaskType)E_CrossBound::Right) {
        this->Velocity.x *= -1;
        this->Position.x = width - this->Size.x;
      }
      if (outType & (BoundMaskType)E_CrossBound::Up) {
        this->Velocity.y *= -1;
        this->Position.y = 0;
      }
      if (outType & (BoundMaskType)E_CrossBound::Down) {
        OnOutOfBound.Broadcast();
      }
    }
  }
}

void Ball::ManipulableHandle(std::array<bool, 1024> &Keys) {
  if (Keys[GLFW_KEY_SPACE]) {
    this->IsStuck = false;
  }
}

ICollision::CollisionInfo_Type Ball::CheckCollision(const GameObject &other) {
  glm::vec2 center(Position + glm::vec2(Radius, Radius));

  glm::vec2 halfSize(other.Size.x / 2.f, other.Size.y / 2.f);

  glm::vec2 boxCenter(other.Position + halfSize);

  glm::vec2 diff = center - boxCenter;

  diff = glm::clamp(diff, -halfSize, halfSize);

  glm::vec2 closest = boxCenter + diff;

  if (glm::length(center - closest) > Radius) {
    return {false, (BoundMaskType)E_CrossBound::Nope};
  }

  BoundMaskType boundMask = (BoundMaskType)E_CrossBound::Nope;
  if (closest.x == other.Position.x) {
    boundMask |= (BoundMaskType)E_CrossBound::Left;
  } else if (closest.x == other.Position.x + other.Size.x) {
    boundMask |= (BoundMaskType)E_CrossBound::Right;
  }
  if (closest.y == other.Position.y) {
    boundMask |= (BoundMaskType)E_CrossBound::Up;
  } else if (closest.y == other.Position.y + other.Size.y) {
    boundMask |= (BoundMaskType)E_CrossBound::Down;
  }

  return {true, boundMask};
}

void Ball::OnCollision(const GameObject &other, CollisionInfo_Type &collisionInfo) {

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
  case E_ObjectType::Paddle: {
    if (this->IsStuck == false) {

      auto colliType = std::get<1>(collisionInfo);
      if (colliType & (BoundMaskType)E_CrossBound::Left) {
        this->Position.x = other.Position.x - this->Size.x;
        this->Velocity.x *= -1;
      } else if (colliType & (BoundMaskType)E_CrossBound::Right) {
        this->Position.x = other.Position.x + other.Size.x;
        this->Velocity.x *= -1;
      }
      if (colliType & (BoundMaskType)E_CrossBound::Up) {
        handleColliWithPaddle(other, true);
      } else if (colliType & (BoundMaskType)E_CrossBound::Down) {
        handleColliWithPaddle(other, false);
      }

      auto paddle = dynamic_cast<const DynamicGOBase &>(other);
      this->Velocity += paddle.Velocity * 0.25f;
    }
    break;
  }
  default:
    break;
  }
}

void Ball::handleColliWithPaddle(const GameObject &other, bool isUp) {
  if (isUp) {
    this->Position.y = other.Position.y - this->Size.y;
  } else {
    this->Position.y = other.Position.y + other.Size.y;
  }
  this->Velocity.y *= -1;

  float length = glm::length(this->Velocity);

  float paddleCentre = other.Position.x + other.Size.x / 2;
  float ballCentre = this->Position.x + this->Radius;
  float distance = ballCentre - paddleCentre;

  float percentage = distance / (other.Size.x / 2);
  const float strength = 2.0f;
  this->Velocity.x *= strength * percentage;
  this->Velocity = glm::normalize(this->Velocity) * length;
}

void Ball::Reset(const Paddle &paddle) {
  this->IsStuck = true;
  this->Position = paddle.Position + glm::vec2(paddle.Size.x / 2.f - Radius, -Radius * 2.f - 0.01f);
  this->Velocity = glm::vec2(100.0f, -350.0f);
}
