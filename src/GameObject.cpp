#include "GameObject.hpp"
#include "SpriteRenderer.hpp"

GameObject::GameObject() : Position(0, 0), Size(1, 1), Color(1.0f), Rotation(0.0f), Sprite(nullptr), IsSolid(false), Destroyed(false) {}

GameObject::GameObject(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color) : Position(position), Size(size), Color(color), Rotation(0.0f), Sprite(&sprite), IsSolid(false), Destroyed(false) {}

void GameObject::Draw(SpriteRenderer &renderer) {
  renderer.DrawSprite(*(this->Sprite), this->Position, this->Size, this->Rotation, this->Color);
}

bool GameObject::CheckCollision(const GameObject &other) {
  bool collisionX = this->Position.x + this->Size.x >= other.Position.x &&
                    other.Position.x + other.Size.x >= this->Position.x;
  bool collisionY = this->Position.y + this->Size.y >= other.Position.y &&
                    other.Position.y + other.Size.y >= this->Position.y;
  return collisionX && collisionY;
}

void GameObject::OnCollision(const GameObject &other) {}
