#include "GameLevel.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include <fstream>
#include <sstream>

void GameLevel::Load(const Path &filePath, int levelWidth, int levelHeight) {
  this->Bricks.clear();
  this->DestroyableCount = 0;
  unsigned int brickCode;
  std::string line;
  std::ifstream fstream(filePath);
  std::vector<std::vector<unsigned int>> tileData;
  std::vector<unsigned int> row;
  if (fstream) {

    while (std::getline(fstream, line)) {
      row.clear();
      std::istringstream sstream(line);
      while (sstream >> brickCode) {
        row.push_back(brickCode);
      }
      tileData.push_back(row);
    }
    if (tileData.size() > 0) {
      this->init(tileData, levelWidth, levelHeight);
    }
  }
}

void GameLevel::init(std::vector<std::vector<unsigned int>> &tileData, int levelWidth, int levelHeight) {
  int height = tileData.size();
  int width = tileData[0].size();
  float unit_width = levelWidth / static_cast<float>(width);
  float unit_height = levelHeight / static_cast<float>(height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      glm::vec2 pos(unit_width * x, unit_height * y);
      glm::vec2 size(unit_width, unit_height);
      switch (tileData[y][x]) {
      case 0u:
        break;
      case 1u: {
        GameObject brick(pos, size, ResourceManager::Instance().GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
        brick.IsSolid = true;
        this->Bricks.push_back(brick);
        break;
      }
      default: {
        glm::vec3 color = glm::vec3(1.0f); // 默认为白色
        if (tileData[y][x] == 2)
          color = glm::vec3(0.2f, 0.6f, 1.0f);
        else if (tileData[y][x] == 3)
          color = glm::vec3(0.0f, 0.7f, 0.0f);
        else if (tileData[y][x] == 4)
          color = glm::vec3(0.8f, 0.8f, 0.4f);
        else if (tileData[y][x] == 5)
          color = glm::vec3(1.0f, 0.5f, 0.0f);
        GameObject brick(pos, size, ResourceManager::Instance().GetTexture("block"), color);
        brick.IsSolid = false;
        this->Bricks.push_back(brick);
        ++DestroyableCount;
        break;
      }
      }
    }
  }
}

void GameLevel::Draw(SpriteRenderer &renderer) {
  for (auto &brick : Bricks) {
    if (brick.Destroyed == false) {
      brick.Draw(renderer);
    }
  }
}

bool GameLevel::IsCompleted() {
  return this->DestroyableCount == 0;
}
