#pragma once

#include "EGameState.hpp"
#include "GameLevel.hpp"
#include "IManipulable.ipp"
#include "InitKey.hpp"
#include <functional>
class Menu : public IManipulable {
public:
  void Render(int width, int height, int level,GameState state);
  void ManipulableHandle(std::array<bool, 1024> &Keys, std::array<bool, 1024> &KeysProcessed) override;
  void Init(InitKey, std::function<std::tuple<GameState &, std::vector<GameLevel> &, int &>(void)> GetGameStateFunc);

private:
  std::function<std::tuple<GameState &, std::vector<GameLevel> &, int &>(void)> GetGameState;
};
