#include "PowerUp.hpp"
#include <string>
#include <unordered_map>
#include <vector>
class PowerUpModule {
public:
  std::vector<PowerUp> PowerUps;
  std::unordered_map<std::string, float> Weights;
  int TotalPowUp;

public:
  PowerUpModule(int totalPowUps = 0) : TotalPowUp(totalPowUps) {}
  void Draw(SpriteRenderer &renderer);
  void Reset();
  void Update(float dt, int width, int height);
  void RandomGenerate(const glm::vec2 &position);
};
