#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
class ParticleObject {
public:
  glm::vec2 Position;
  glm::vec2 Velocity;
  glm::vec4 Color;
  float Life;

  ParticleObject() : Position(0.f), Velocity(0.f), Color(1.f), Life(0.f) {};
};