#include "GameObject.hpp"
#include "Global.cpp"
#include "SpriteRenderer.hpp"
class GameLevel {
public:
  std::vector<GameObject> Bricks;
  int DestroyableCount = 0;

public:
  GameLevel() = default;
  void Load(const Path &filePath, int levelWidth, int levelHeight);
  void Draw(SpriteRenderer &renderer);
  
  bool IsCompleted();
  void init(std::vector<std::vector<unsigned int>> &tileData, int levelWidth, int levelHeight);
};