#include "DynamicGOBase.hpp"

DynamicGOBase::OutOfBoundMask DynamicGOBase::isObjectOutOfBound(int width, int height) {
  OutOfBoundMask result = (OutOfBoundMask)E_OutOfBound::Nope;
  if (this->Position.x < 0) {
    result |= (OutOfBoundMask)E_OutOfBound::Left;
  }
  if (this->Position.x + this->Size.x > width) {
    result |= (OutOfBoundMask)E_OutOfBound::Right;
  }
  if (this->Position.y < 0) {
    result |= (OutOfBoundMask)E_OutOfBound::Up;
  }
  if (this->Position.y + this->Size.y > height) {
    result |= (OutOfBoundMask)E_OutOfBound::Down;
  }
  return result;
}