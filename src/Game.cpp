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

  initParticleSystem();
}

void Game::Render() {
  if (this->State == GameState::GAME_ACTIVE) {
    SpriteRenderer::Instance().DrawSprite(ResourceManager::Instance().GetTexture("background"), glm::vec2(0.f, 0.f), glm::vec2(this->Width, this->Height), 0.f);

    this->Levels[this->curr_level].Draw(SpriteRenderer::Instance());

    PaddleObj->Draw(SpriteRenderer::Instance());
    ParticleSystem->Render();
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
    DoCollision();
    ParticleSystem->Update(dt, *BallObj);
  }
}

void Game::initShader(Camera &mainCamera, Path &projroot) {
  {
    auto &shader = ResourceManager::Instance().LoadShader("default_sprite_shader", projroot / "src/shader/sprite.vert", projroot / "src/shader/sprite.frag");
    shader.Use();
    shader.SetUniform("projection", mainCamera.GetProjectionMatrix_Ortho());
    shader.SetUniform("image", 0);
  }
  {
    auto &shader = ResourceManager::Instance().LoadShader("default_particle_shader", projroot / "src/shader/particle.vert", projroot / "src/shader/particle.frag");
    shader.Use();
    shader.SetUniform("projection", mainCamera.GetProjectionMatrix_Ortho());
    shader.SetUniform("sprite", 1);
  }
}

void Game::initSprite(Path &projroot) {
  SpriteRenderer::Instance().SetShader(ResourceManager::Instance().GetShader("default_sprite_shader"));
  ResourceManager::Instance().LoadTexture2D("face", projroot / "res/texture/awesomeface.png", true);
  ResourceManager::Instance().LoadTexture2D("background", projroot / "res/texture/background.jpg", false);
  ResourceManager::Instance().LoadTexture2D("block", projroot / "res/texture/block.png", false);
  ResourceManager::Instance().LoadTexture2D("block_solid", projroot / "res/texture/block_solid.png", true);
  ResourceManager::Instance().LoadTexture2D("paddle", projroot / "res/texture/paddle.png", true);
  ResourceManager::Instance().LoadTexture2D("particle", projroot / "res/texture/particle.png");
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
  const glm::vec2 initPos = PaddleObj->Position + glm::vec2(PaddleObj->Size.x / 2.f - initRadius, -initRadius * 2.f - 0.01f);
  BallObj = std::make_unique<Ball>(initRadius, true, initPos, initSize, faceTex, glm::vec3(1.f), initVelocity);
  BallObj->SetLaunchPaddle(*(this->PaddleObj));
  auto GameFail = [this]() { this->Fail(); };
  BallObj->OnOutOfBound.Subscribe(GameFail);
}

void Game::initParticleSystem() {
  const int initTotal = 500;
  const int initNew = 2;
  const glm::vec2 initOffset(BallObj->Radius / 4);
  auto &shader = ResourceManager::Instance().GetShader("default_particle_shader");
  auto &sprite = ResourceManager::Instance().GetTexture("particle");
  ParticleSystem = std::make_unique<ParticleGenertor>(initTotal, initNew, initOffset, shader, sprite);
}
void Game::DoCollision() {
  for (auto &brick : Levels[curr_level].Bricks) {
    // TODO
    if (!brick.Destroyed) {
      if (auto colliInfo = BallObj->CheckCollision(brick); std::get<0>(colliInfo)) {
        brick.OnCollision(*BallObj, colliInfo);
        BallObj->OnCollision(brick, colliInfo);
        if (brick.IsSolid == false) {
          this->Levels[this->curr_level].DestroyableCount--;
          if (this->Levels[curr_level].IsCompleted()) {
            ++this->curr_level;
            Reset();
          }
        }
      }
      if (auto colliInfo = PaddleObj->CheckCollision(brick); std::get<0>(colliInfo)) {
        PaddleObj->OnCollision(brick, colliInfo);
      }
    }
  }
  if (auto colliInfo = BallObj->CheckCollision(*PaddleObj); std::get<0>(colliInfo)) {
    BallObj->OnCollision(*PaddleObj, colliInfo);
  }
}
void Game::Reset() {
  for (auto &lv : Levels) {
    lv.Reset();
  }
  PaddleObj->Reset(this->Width, this->Height);
  BallObj->Reset(*PaddleObj);
}

void Game::Fail() {
  Reset();
}
