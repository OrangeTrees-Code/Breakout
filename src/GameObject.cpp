#include "GameObject.hpp"
#include "SpriteRenderer.hpp"

GameObject::GameObject() : Position(0, 0), Size(1, 1), Color(1.0f), Rotation(0.0f), Sprite(nullptr), IsSolid(false), Destroyed(false), ObjType(E_ObjectType::GameObject) {}

GameObject::GameObject(E_ObjectType type, const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color) : Position(position), Size(size), Color(color), Rotation(0.0f), Sprite(&sprite), IsSolid(false), Destroyed(false), ObjType(type) {}

GameObject::GameObject(E_ObjectType type) : ObjType(type), Position(0, 0), Size(1, 1), Color(1.0f), Rotation(0.0f), Sprite(nullptr), IsSolid(false), Destroyed(false) {};

std::unique_ptr<GameObject> GameObject::Clone() const {
  return std::make_unique<GameObject>(*this);
}

void GameObject::Draw(SpriteRenderer &renderer) {
  renderer.DrawSprite(*(this->Sprite), this->Position, this->Size, this->Rotation, this->Color);
}

// void GameObject::OnCollision(const GameObject &other) {}
