#include "Game.hpp"
#include "Camera.hpp"
#include "Paddle.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <ranges>
#include <string>

void Game::Init(Camera &mainCamera) {
  auto &projroot = ResourceManager::Instance().ProjRoot;

  initShader(mainCamera, projroot);

  initSprite(projroot);

  initLevel(projroot);

  initPaddle();

  initBall();
}

void Game::Render() {
  if (this->State == GameState::GAME_ACTIVE) {
    SpriteRenderer::Instance().DrawSprite(ResourceManager::Instance().GetTexture("background"), glm::vec2(0.f, 0.f), glm::vec2(this->Width, this->Height), 0.f);

    this->Levels[this->curr_level].Draw(SpriteRenderer::Instance());

    PaddleObj->Draw(SpriteRenderer::Instance());
    BallObj->Draw(SpriteRenderer::Instance());
  }
}

void Game::ProcessInput() {
  if (this->State == GameState::GAME_ACTIVE) {
    PaddleObj->ManipulableHandle(Keys);
    BallObj->ManipulableHandle(Keys);
  }
}

void Game::Update(GLfloat dt) {
  if (this->State == GameState::GAME_ACTIVE) {
    this->PaddleObj->Move(dt, this->Width, this->Height);
    this->BallObj->Move(dt, this->Width, this->Height);
  }
}

void Game::initShader(Camera &mainCamera, Path &projroot) {
  auto &shader = ResourceManager::Instance().LoadShader("default_sprite_shader", projroot / "src/shader/sprite.vert", projroot / "src/shader/sprite.frag");
  SpriteRenderer::Instance().SetShader(shader);
  shader.Use();
  shader.SetUniform("projection", mainCamera.GetProjectionMatrix_Ortho());
  shader.SetUniform("image", 0);
}

void Game::initSprite(Path &projroot) {
  ResourceManager::Instance().LoadTexture2D("face", projroot / "res/texture/awesomeface.png", true);
  ResourceManager::Instance().LoadTexture2D("background", projroot / "res/texture/background.jpg", false);
  ResourceManager::Instance().LoadTexture2D("block", projroot / "res/texture/block.png", false);
  ResourceManager::Instance().LoadTexture2D("block_solid", projroot / "res/texture/block_solid.png", true);
  ResourceManager::Instance().LoadTexture2D("paddle", projroot / "res/texture/paddle.png", true);
}
void Game::initLevel(Path &projroot) {
  for (int i : std::ranges::views::iota(0, 4)) {
    GameLevel lv;
    lv.Load(projroot / ("res/config/lv" + std::to_string(i + 1) + ".lvl"), this->Width, this->Height / 2);
    Levels.push_back(lv);
  }
}
void Game::initPaddle() {
  const glm::vec2 initSize(100.f, 20.f);
  const glm::vec2 initPos((float)this->Width / 2 - initSize.x / 2, (float)this->Height - initSize.y);
  auto &paddleTex = ResourceManager::Instance().GetTexture("paddle");
  PaddleObj = std::make_unique<Paddle>(initPos, initSize, paddleTex);
}
void Game::initBall() {
  const glm::vec2 initVelocity(100.0f, -350.0f);
  const float initRadius = 12.5f;
  auto &faceTex = ResourceManager::Instance().GetTexture("face");
  const glm::vec2 initSize(initRadius * 2.f);
  const glm::vec2 initPos = PaddleObj->Position + glm::vec2(PaddleObj->Size.x / 2.f - initRadius, -initRadius * 2.f);
  BallObj = std::make_unique<Ball>(initRadius, true, initPos, initSize, faceTex, glm::vec3(1.f), initVelocity);
  BallObj->SetLaunchPaddle(*(this->PaddleObj));
}
