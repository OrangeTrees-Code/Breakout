#pragma once

#include "DynamicGOBase.hpp"
#include "GameObject.hpp"

class ICollision {
public:
  using BoundMaskType = unsigned int;
  using CollisionInfo_Type = std::tuple<bool, BoundMaskType>;

public:
  virtual CollisionInfo_Type CheckCollision(const GameObject &other) { return {false, (BoundMaskType)E_CrossBound::Nope}; }
  virtual void OnCollision(const GameObject &other, CollisionInfo_Type &collisionInfo) {};
};