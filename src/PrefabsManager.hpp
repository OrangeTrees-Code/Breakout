#pragma once

#include "GameObject.hpp"
#include "InitKey.hpp"
#include <memory>
#include <string>
#include <unordered_map>
class PrefabsManager {
public:
  static PrefabsManager &Instance();
  static void Init(InitKey);
  void SaveAsPrefab(const std::string &name, const GameObject &gameObject);
  std::unique_ptr<GameObject> Instantiate(const std::string &name);
  std::unique_ptr<GameObject> Instantiate(const std::string &name, const glm::vec2 &position);

private:
  PrefabsManager() = default;

private:
  static std::unique_ptr<PrefabsManager> instance_;

  std::unordered_map<std::string, std::unique_ptr<GameObject>> entities_;
};
