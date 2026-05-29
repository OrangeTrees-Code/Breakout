#include "DynamicGOBase.hpp"

DynamicGOBase::BoundMaskType DynamicGOBase::isObjectOutOfBound(int width, int height) {
  BoundMaskType result = (BoundMaskType)E_CrossBound::Nope;
  if (this->Position.x < 0) {
    result |= (BoundMaskType)E_CrossBound::Left;
  }
  if (this->Position.x + this->Size.x > width) {
    result |= (BoundMaskType)E_CrossBound::Right;
  }
  if (this->Position.y < 0) {
    result |= (BoundMaskType)E_CrossBound::Up;
  }
  if (this->Position.y + this->Size.y > height) {
    result |= (BoundMaskType)E_CrossBound::Down;
  }
  return result;
}

