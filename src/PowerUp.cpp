#include "PowerUp.hpp"
#include "PostProcessor.hpp"

void PowerUp::Move(float deltaTime, int width, int height) {
  if (!this->Destroyed) {
    this->Position += this->Velocity * deltaTime;
    if (auto outType = isObjectOutOfBound(width, height); outType != (BoundMaskType)E_CrossBound::Nope) {
      if (outType & (BoundMaskType)E_CrossBound::Down) {
        this->Destroyed = true;
      }
    }
  }
}

void PowerUp::Init(InitKey, std::function<Ball &(void)> GetBallFunc, std::function<Paddle &(void)> GetPaddleFunc) {
  GetBall = GetBallFunc;
  GetPaddle = GetPaddleFunc;
}

void PowerUp::Active() {
  auto &paddle = GetPaddle();
  auto &ball = GetBall();
  switch (Type) {
  case E_PowerUpType::Speed: {
    ball.Velocity *= 1.2;
    break;
  }
  case E_PowerUpType::Sticky: {
    paddle.IsSticky = true;
    break;
  }
  case E_PowerUpType::PassThrough: {
    ball.IsPassThrough = true;
    break;
  }
  case E_PowerUpType::PadSizeIncrease: {
    paddle.Size.x = std::min(paddle.Size.x + 50.f, paddle.MaxLength);
    break;
  }
  case E_PowerUpType::Confuse: {
    auto &instance = PostProcessor::Instance();
    if (instance.Confuse == false) {
      instance.StartEffect(E_EffectType::Confuse, this->Duration);
    }
    break;
  }
  case E_PowerUpType::Chaos: {
    auto &instance = PostProcessor::Instance();
    if (instance.Chaos == false) {
      instance.StartEffect(E_EffectType::Chaos, this->Duration);
    }
    break;
  }
  default:
    break;
  }
}

void PowerUp::Deactive() {
  auto &paddle = GetPaddle();
  auto &ball = GetBall();
  switch (Type) {
  case E_PowerUpType::Sticky: {
    paddle.IsSticky = false;
    ball.IsStuck = false;
    break;
  }
  case E_PowerUpType::PassThrough: {
    ball.IsPassThrough = false;
    break;
  }
  default:
    break;
  }
}

std::unique_ptr<GameObject> PowerUp::Clone() const {
  return std::make_unique<PowerUp>(*this);
}

void PowerUp::OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) {
  if (other.ObjType == E_ObjectType::Paddle) {
    this->Actived = true;
    this->Destroyed = true;
  }
}

void PowerUp::Update(float deltaTime) {
  if (this->Actived) {
    this->Duration -= deltaTime;
    if (this->Duration <= 0.f) {
      this->Actived = false;
      Deactive();
    }
  }
}