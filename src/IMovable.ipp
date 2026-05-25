#pragma once

class IMovable {
public:
  virtual void Move(float deltaTime, int width, int height) = 0;
  virtual ~IMovable() = default;
};
