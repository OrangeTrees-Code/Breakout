#include "Game.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <ranges>
#include <string>

void Game::Init(Camera &mainCamera) {
  auto &projroot = ResourceManager::Instance().ProjRoot;
  auto &shader = ResourceManager::Instance().LoadShader("default_sprite_shader", projroot / "src/shader/sprite.vert", projroot / "src/shader/sprite.frag");
  SpriteRenderer::Instance().SetShader(shader);
  shader.Use();
  shader.SetUniform("projection", mainCamera.GetProjectionMatrix_Ortho());
  shader.SetUniform("image", 0);

  ResourceManager::Instance().LoadTexture2D("face", projroot / "res/texture/awesomeface.png", true);
  ResourceManager::Instance().LoadTexture2D("background", projroot / "res/texture/background.jpg", false);
  ResourceManager::Instance().LoadTexture2D("block", projroot / "res/texture/block.png", false);
  ResourceManager::Instance().LoadTexture2D("block_solid", projroot / "res/texture/block_solid.png", true);

  for (int i : std::ranges::views::iota(0, 4)) {
    GameLevel lv;
    lv.Load(projroot / ("res/config/lv" + std::to_string(i + 1) + ".lvl"), this->Width, this->Height / 2);
    Levels.push_back(lv);
  }

  glm::vec2 initPlayerSize(100.f, 20.f);
  auto &paddle = ResourceManager::Instance().LoadTexture2D("paddle", projroot / "res/texture/paddle.png", true);
  glm::vec2 initPlayerPos((float)this->Width / 2 - initPlayerSize.x / 2, (float)this->Height - initPlayerSize.y);
  PlayerObject = std::make_unique<Player>(initPlayerPos, initPlayerSize, paddle);
}

void Game::Render() {
  if (this->State == GameState::GAME_ACTIVE) {
    SpriteRenderer::Instance().DrawSprite(ResourceManager::Instance().GetTexture("background"), glm::vec2(0.f, 0.f), glm::vec2(this->Width, this->Height), 0.f);

    this->Levels[this->curr_level].Draw(SpriteRenderer::Instance());

    PlayerObject->Draw(SpriteRenderer::Instance());
  }
}

void Game::ProcessInput() {
  PlayerObject->Velocity = glm::vec2(0.f);
  if (this->Keys[GLFW_KEY_A]) {
    PlayerObject->Velocity.x = -500.f;
  }
  if (this->Keys[GLFW_KEY_D]) {
    PlayerObject->Velocity.x = 500.f;
  }
  if (this->Keys[GLFW_KEY_W]) {
    PlayerObject->Velocity.y = -500.f;
  }
  if (this->Keys[GLFW_KEY_S]) {
    PlayerObject->Velocity.y = 500.f;
  }
}

void Game::Update(GLfloat dt) {
  if (PlayerObject->Velocity != glm::vec2(0.f, 0.f)) {
    PlayerObject->Position += PlayerObject->Velocity * dt;
    if (OutOfBoundMask outType = isPlayerOutOfBound(); outType != (OutOfBoundMask)E_OutOfBound::Nope) {
      if (outType & (OutOfBoundMask)E_OutOfBound::Left) {
        PlayerObject->Position.x = 0;
      }
      if (outType & (OutOfBoundMask)E_OutOfBound::Right) {
        PlayerObject->Position.x = this->Width - PlayerObject->Size.x;
      }
      if (outType & (OutOfBoundMask)E_OutOfBound::Up) {
        PlayerObject->Position.y = 0;
      }
      if (outType & (OutOfBoundMask)E_OutOfBound::Down) {
        PlayerObject->Position.y = this->Height - PlayerObject->Size.y;
      }
    }
  }
}

Game::OutOfBoundMask Game::isPlayerOutOfBound() {
  OutOfBoundMask result = (OutOfBoundMask)E_OutOfBound::Nope;
  if (PlayerObject->Position.x < 0) {
    result |= (OutOfBoundMask)E_OutOfBound::Left;
  }
  if (PlayerObject->Position.x + PlayerObject->Size.x > this->Width) {
    result |= (OutOfBoundMask)E_OutOfBound::Right;
  }
  if (PlayerObject->Position.y < 0) {
    result |= (OutOfBoundMask)E_OutOfBound::Up;
  }
  if (PlayerObject->Position.y + PlayerObject->Size.y > this->Height) {
    result |= (OutOfBoundMask)E_OutOfBound::Down;
  }
  return result;
}
