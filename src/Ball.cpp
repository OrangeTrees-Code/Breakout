#include "Ball.hpp"
#include "DynamicGOBase.hpp"
#include "GameObject.hpp"
#include "Paddle.hpp"
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

void Ball::Move(float deltaTime, int width, int height) {
  if (shouldUpdateOffset_) {
    offsetToPaddle = this->Position - launchPaddle_->Position;
    shouldUpdateOffset_ = false;
  }
  if (this->IsStuck && this->launchPaddle_ != nullptr) {
    this->Position = launchPaddle_->Position + offsetToPaddle;
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

void Ball::OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) {

  switch (other.ObjType) {
  case E_ObjectType::Brick: {
    auto colliType = std::get<1>(collisionInfo);
    handleColliWithObject(other, colliType);
    break;
  }
  case E_ObjectType::Paddle: {
    const auto &paddle = dynamic_cast<const Paddle &>(other);
    auto colliType = std::get<1>(collisionInfo);

    if (this->IsStuck == false) {
      if (paddle.IsSticky) {
        handleColliWithPaddle(other, colliType);
        shouldUpdateOffset_ = true;
        IsStuck = true;
      } else {
        handleColliWithPaddle(other, colliType);
        this->Velocity += paddle.Velocity * 0.25f;
      }
    }
    break;
  }
  default:
    break;
  }
}

void Ball::handleColliWithObjectX(const GameObject &other, BoundMaskType colliType) {
  if (colliType & (BoundMaskType)E_CrossBound::Left) {
    this->Position.x = other.Position.x - this->Size.x;
    this->Velocity.x *= -1;
  } else if (colliType & (BoundMaskType)E_CrossBound::Right) {
    this->Position.x = other.Position.x + other.Size.x;
    this->Velocity.x *= -1;
  }
}

void Ball::handleColliWithObjectY(const GameObject &other, BoundMaskType colliType) {
  if (colliType & (BoundMaskType)E_CrossBound::Up) {
    this->Position.y = other.Position.y - this->Size.y;
    this->Velocity.y *= -1;
  } else if (colliType & (BoundMaskType)E_CrossBound::Down) {
    this->Position.y = other.Position.y + other.Size.y;
    this->Velocity.y *= -1;
  }
}

void Ball::handleColliWithObject(const GameObject &other, BoundMaskType colliType) {
  handleColliWithObjectX(other, colliType);
  handleColliWithObjectY(other, colliType);
}

void Ball::handleColliWithPaddle(const GameObject &other, BoundMaskType colliType) {
  handleColliWithObjectX(other, colliType);
  if (colliType & (BoundMaskType)E_CrossBound::Up) {
    handleColliWithPaddleY(other, true);
  } else if (colliType & (BoundMaskType)E_CrossBound::Down) {
    handleColliWithPaddleY(other, false);
  }
}

void Ball::handleColliWithPaddleY(const GameObject &other, bool isUp) {
  if (isUp) {
    this->Position.y = other.Position.y - this->Size.y;
  } else {
    this->Position.y = other.Position.y + other.Size.y;
  }
  this->Velocity.y *= -1;

  float length = glm::length(this->Velocity);
  if (length <= 0.0001f) {
    this->Velocity = glm::vec2(0.f, isUp ? -350.f : 350.f);
    length = glm::length(this->Velocity);
  }

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
  this->shouldUpdateOffset_ = true;
  this->Velocity = glm::vec2(100.0f, -350.0f);
  this->IsPassThrough = false;
}

void Ball::resetOffsetWithPaddle_() {
  shouldUpdateOffset_ = true;
}

void Ball::SetLaunchPaddle(Paddle &paddle, bool isSyncOffset) {
  launchPaddle_ = &paddle;
  if (isSyncOffset) {
    offsetToPaddle = this->Position - launchPaddle_->Position;
  }
}
