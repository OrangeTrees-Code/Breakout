#include "Brick.hpp"

void Brick::OnCollision(const GameObject &other, CollisionInfo_Type &collisionInfo) {
  if (!IsSolid) {
    this->Destroyed = true;
  }
}

void Brick::Reset() {
  this->Destroyed = false;
}