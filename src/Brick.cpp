#include "Brick.hpp"

void Brick::OnCollision(const GameObject &other) {
  if (!IsSolid) {
    this->Destroyed = true;
  }
}