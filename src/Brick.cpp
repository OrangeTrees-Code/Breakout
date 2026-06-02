#include "Brick.hpp"

void Brick::OnCollision(GameObject &other, CollisionInfo_Type &collisionInfo) {
  if (!IsSolid) {
    this->Destroyed = true;
  }
}

void Brick::Reset() {
  this->Destroyed = false;
}