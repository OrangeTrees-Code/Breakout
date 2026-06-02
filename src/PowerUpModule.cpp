#include "PowerUpModule.hpp"
#include "PrefabsManager.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>
void PowerUpModule::Draw(SpriteRenderer &renderer) {
  std::for_each(PowerUps.begin(), PowerUps.end(), [](PowerUp &powup) {
    if (!powup.Destroyed) {
      powup.Draw(SpriteRenderer::Instance());
    }
  });
}

void PowerUpModule::Reset() {
  this->PowerUps.clear();
}

void PowerUpModule::Update(float dt,int width,int height) {
  for (auto &powup : PowerUps) {
    powup.Move(dt, width, height);
    powup.Update(dt);
  }

  auto removeBegin = std::remove_if(PowerUps.begin(), PowerUps.end(), [](const PowerUp &powup) {
    return powup.Destroyed && !powup.Actived;
  });
  PowerUps.erase(removeBegin, PowerUps.end());
}

void PowerUpModule::RandomGenerate(const glm::vec2 &position) {
  float totalWeight = 0.f;
  for (const auto &[_, weight] : Weights) {
    if (weight > 0.f) {
      totalWeight += weight;
    }
  }

  if (totalWeight <= 0.f) {
    return;
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(0.f, totalWeight);
  float roll = dist(gen);

  for (const auto &[name, weight] : Weights) {
    if (weight <= 0.f) {
      continue;
    }

    roll -= weight;
    if (roll <= 0.f) {
      auto gameObject = PrefabsManager::Instance().Instantiate(name, position);
      auto *powerUp = dynamic_cast<PowerUp *>(gameObject.get());
      if (!powerUp) {
        throw std::runtime_error("ERROR::POWERUPMODULE::RandomGenerate: " + name + " is not a PowerUp prefab!");
      }

      PowerUps.push_back(std::move(*powerUp));
      return;
    }
  }
}
