#include "Menu.hpp"
#include "PostProcessor.hpp"
#include "TextRenderer.hpp"

void Menu::Init(InitKey, std::function<std::tuple<GameState &, std::vector<GameLevel> &, int &>(void)> GetGameStateFunc) {
  GetGameState = GetGameStateFunc;
}

void Menu::Render(int width, int height, int level, GameState state) {
  switch (state) {
  case GameState::GAME_MENU: {
    TextRenderer::Instance().RenderText("Press ENTER to start", width / 2.f - 100.f, height / 2.f, 1.0f);
    TextRenderer::Instance().RenderText("Press W or S to select level", width / 2.f - 90.f, height / 2.f + 20.0f, 0.75f);
    TextRenderer::Instance().RenderText(std::to_string(level), width / 2.f, height / 2.f + 40.f, 1.f);
    break;
  }
  case GameState::GAME_WIN: {
    TextRenderer::Instance().RenderText("You WON!!!", width / 2.f - 60.f, height / 2.f, 1.5f, glm::vec3(0.0, 1.0, 0.0));
    TextRenderer::Instance().RenderText("Press ENTER to retry or ESC to quit", width / 2.f - 250.f, height / 2.f + 40.0f, 1.5f, glm::vec3(1.f, 1.f, 0.f));
    break;
  }
  default:
    break;
  }
}

void Menu::ManipulableHandle(std::array<bool, 1024> &Keys, std::array<bool, 1024> &KeysProcessed) {
  auto [State, Levels, CurrLevel] = GetGameState();
  switch (State) {
  case GameState::GAME_MENU: {
    if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
      State = GameState::GAME_ACTIVE;
      KeysProcessed[GLFW_KEY_ENTER] = true;
    }
    if (Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W]) {
      CurrLevel = (CurrLevel + 1) % Levels.size();
      KeysProcessed[GLFW_KEY_W] = true;
    }
    if (Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S]) {
      CurrLevel = CurrLevel == 0 ? Levels.size() - 1 : CurrLevel - 1;
      KeysProcessed[GLFW_KEY_S] = true;
    }
    break;
  }
  case GameState::GAME_WIN: {
    if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
      KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
      PostProcessor::Instance().EndEffect(E_EffectType::Chaos);
      State = GameState::GAME_MENU;
    }
    break;
  }
  default:
    break;
  }
}
