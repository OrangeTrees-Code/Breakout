#include "GameObject.hpp"
class Brick : public GameObject {
public:
  Brick() : GameObject() {};
  Brick(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f)) : GameObject(position, size, sprite, color) {};

  void OnCollision(const GameObject &other) override;

};