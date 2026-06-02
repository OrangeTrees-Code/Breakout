#include "PrefabsManager.hpp"

std::unique_ptr<PrefabsManager> PrefabsManager::instance_ = nullptr;

PrefabsManager &PrefabsManager::Instance() {
  if (!instance_) {
    throw std::runtime_error("PrefabsManager has not been initialized");
  }
  return *instance_;
}

void PrefabsManager::Init(InitKey) {
  instance_ = std::unique_ptr<PrefabsManager>(new PrefabsManager());
}

void PrefabsManager::SaveAsPrefab(const std::string &name, const GameObject &gameObject) {
  if (entities_.contains(name)) {
    throw std::runtime_error("ERROR::PREFABSMANAGER::SaveAsPrefab: " + name + " already exist!");
  }

  entities_[name] = gameObject.Clone();
}

std::unique_ptr<GameObject> PrefabsManager::Instantiate(const std::string &name) {
  if (!entities_.contains(name)) {
    throw std::runtime_error("ERROR::PREFABSMANAGER::Instantiate: " + name + " doesn't exist!");
  }

  return entities_[name]->Clone();
}

std::unique_ptr<GameObject> PrefabsManager::Instantiate(const std::string &name, const glm::vec2 &position) {
  if (!entities_.contains(name)) {
    throw std::runtime_error("ERROR::PREFABSMANAGER::Instantiate: " + name + " doesn't exist!");
  }

  std::unique_ptr<GameObject> obj = entities_[name]->Clone();
  obj->Position = position;
  return obj;
}
