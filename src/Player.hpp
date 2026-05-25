#include "GameObject.hpp"
class Player : public GameObject {
public:
  glm::vec2 Velocity;

public:
  Player() : GameObject(), Velocity(0.f, 0.f) {};
  Player(const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : GameObject(position, size, sprite, color), Velocity(velocity) {};
  ~Player() = default;
};
