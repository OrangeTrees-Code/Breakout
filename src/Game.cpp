#include "Game.hpp"
#include "Camera.hpp"
#include "Paddle.hpp"
#include "ParticleGenerator.hpp"
#include "PostProcessor.hpp"
#include "PrefabsManager.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <memory>
#include <ranges>
#include <string>

void Game::Init(Camera &mainCamera, int framebuffer_width, int framebuffer_height) {
  auto &projroot = ResourceManager::Instance().ProjRoot;

  initShader(mainCamera, projroot);

  initSprite(projroot, framebuffer_width, framebuffer_height);

  initLevel(projroot);

  initPaddle();

  initBall();

  initParticleSystem();

  initPostProcessor(framebuffer_width, framebuffer_height);

  initPrefabsManager();

  initPowerUpSystem();
}

void Game::Render() {
  if (this->State == GameState::GAME_ACTIVE) {
    PostProcessor::Instance().BeginRender();
    {
      SpriteRenderer::Instance().DrawSprite(ResourceManager::Instance().GetTexture("background"), glm::vec2(0.f, 0.f), glm::vec2(this->Width, this->Height), 0.f);

      this->Levels[this->curr_level].Draw(SpriteRenderer::Instance());

      PaddleObj->Draw(SpriteRenderer::Instance());
      ParticleGenerator::Instance().Render();
      BallObj->Draw(SpriteRenderer::Instance());
      PowerUpSystem.Draw(SpriteRenderer::Instance());
    }
    PostProcessor::Instance().EndRender();

    PostProcessor::Instance().Render(glfwGetTime());
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
    ParticleGenerator::Instance().Update(dt, *BallObj);
    PostProcessor::Instance().Update(dt);
    PowerUpSystem.Update(dt, this->Width, this->Height);
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
  {
    auto &shader = ResourceManager::Instance().LoadShader("default_post_processing_shader", projroot / "src/shader/post_processing.vert", projroot / "src/shader/post_processing.frag");
  }
}

void Game::initSprite(Path &projroot, int framebuffer_width, int framebuffer_height) {
  SpriteRenderer::Init(InitKey(), ResourceManager::Instance().GetShader("default_sprite_shader"));
  ResourceManager::Instance().LoadTexture2D("face", projroot / "res/texture/awesomeface.png", true);
  ResourceManager::Instance().LoadTexture2D("background", projroot / "res/texture/background.jpg", false);
  ResourceManager::Instance().LoadTexture2D("block", projroot / "res/texture/block.png", false);
  ResourceManager::Instance().LoadTexture2D("block_solid", projroot / "res/texture/block_solid.png", true);
  ResourceManager::Instance().LoadTexture2D("paddle", projroot / "res/texture/paddle.png", true);
  ResourceManager::Instance().LoadTexture2D("particle", projroot / "res/texture/particle.png");
  ResourceManager::Instance().LoadTexture2D("speed", projroot / "res/texture/PowerUps/powerup_speed.png", true);
  ResourceManager::Instance().LoadTexture2D("confuse", projroot / "res/texture/PowerUps/powerup_confuse.png", true);
  ResourceManager::Instance().LoadTexture2D("passthrough", projroot / "res/texture/PowerUps/powerup_passthrough.png", true);
  ResourceManager::Instance().LoadTexture2D("sticky", projroot / "res/texture/PowerUps/powerup_sticky.png", true);
  ResourceManager::Instance().LoadTexture2D("increase", projroot / "res/texture/PowerUps/powerup_increase.png", true);
  ResourceManager::Instance().LoadTexture2D("chaos", projroot / "res/texture/PowerUps/powerup_chaos.png", true);

  ResourceManager::Instance().LoadEmptyTextureBuffer("default_empty_texture_buffer", framebuffer_width, framebuffer_height);
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
  PaddleObj->EffectColor = glm::vec3(1.0f, 0.5f, 1.0f);
  PaddleObj->MaxLength = this->Width;
}
void Game::initBall() {
  const glm::vec2 initVelocity(100.0f, -350.0f);
  const float initRadius = 12.5f;
  auto &faceTex = ResourceManager::Instance().GetTexture("face");
  const glm::vec2 initSize(initRadius * 2.f);
  const glm::vec2 initPos = PaddleObj->Position + glm::vec2(PaddleObj->Size.x / 2.f - initRadius, -initRadius * 2.f - 0.01f);
  BallObj = std::make_unique<Ball>(initRadius, true, initPos, initSize, faceTex, glm::vec3(1.f), initVelocity);
  BallObj->SetLaunchPaddle(*(this->PaddleObj), true);
  auto GameFail = [this]() { this->Fail(); };
  BallObj->OnOutOfBound.Subscribe(GameFail);
  BallObj->EffectColor = glm::vec3(1.0f, 0.5f, 0.5f);
}

void Game::initParticleSystem() {
  const int initTotal = 500;
  const int initNew = 2;
  const glm::vec2 initOffset(BallObj->Radius / 2);
  auto &shader = ResourceManager::Instance().GetShader("default_particle_shader");
  auto &sprite = ResourceManager::Instance().GetTexture("particle");
  ParticleGenerator::Init(InitKey(), initTotal, initNew, initOffset, shader, sprite);
}

void Game::initPostProcessor(int framebuffer_width, int framebuffer_height) {
  auto &shader = ResourceManager::Instance().GetShader("default_post_processing_shader");
  auto &texture = ResourceManager::Instance().GetTexture("default_empty_texture_buffer");

  PostProcessor::Init(InitKey(), framebuffer_width, framebuffer_height, shader, texture);
}

void Game::initPrefabsManager() {
  PrefabsManager::Init(InitKey());
  glm::vec2 initVelocity(0.0f, 150.0f);
  glm::vec2 initSize(60, 20);
  auto GetBall = [this]() -> Ball & { return *(this->BallObj); };
  auto GetPaddle = [this]() -> Paddle & { return *(this->PaddleObj); };
  {
    auto &tex = ResourceManager::Instance().GetTexture("speed");
    PowerUp PowUP(E_PowerUpType::Speed, 0.f, initSize, tex, glm::vec3(0.5f, 0.5f, 1.0f), initVelocity);
    PowUP.Init(InitKey(), GetBall, GetPaddle);

    PrefabsManager::Instance().SaveAsPrefab("PowerUp_Speed", PowUP);
  }
  {
    auto &tex = ResourceManager::Instance().GetTexture("sticky");
    PowerUp PowUP(E_PowerUpType::Sticky, 20.f, initSize, tex, glm::vec3(1.0f, 0.5f, 1.0f), initVelocity);
    PowUP.Init(InitKey(), GetBall, GetPaddle);

    PrefabsManager::Instance().SaveAsPrefab("PowerUp_Sticky", PowUP);
  }
  {
    auto &tex = ResourceManager::Instance().GetTexture("passthrough");
    PowerUp PowUP(E_PowerUpType::PassThrough, 10.f, initSize, tex, glm::vec3(0.5f, 1.0f, 0.5f), initVelocity);
    PowUP.Init(InitKey(), GetBall, GetPaddle);

    PrefabsManager::Instance().SaveAsPrefab("PowerUp_PassThrough", PowUP);
  }
  {
    auto &tex = ResourceManager::Instance().GetTexture("increase");
    PowerUp PowUP(E_PowerUpType::PadSizeIncrease, 0.f, initSize, tex, glm::vec3(1.0f, 0.6f, 0.4), initVelocity);
    PowUP.Init(InitKey(), GetBall, GetPaddle);

    PrefabsManager::Instance().SaveAsPrefab("PowerUp_Increase", PowUP);
  }
  {
    auto &tex = ResourceManager::Instance().GetTexture("confuse");
    PowerUp PowUP(E_PowerUpType::Confuse, 15.f, initSize, tex, glm::vec3(1.0f, 0.3f, 0.3f), initVelocity);
    PowUP.Init(InitKey(), GetBall, GetPaddle);

    PrefabsManager::Instance().SaveAsPrefab("PowerUp_Confuse", PowUP);
  }
  {
    auto &tex = ResourceManager::Instance().GetTexture("chaos");
    PowerUp PowUP(E_PowerUpType::Chaos, 15.f, initSize, tex, glm::vec3(0.9f, 0.25f, 0.25f), initVelocity);
    PowUP.Init(InitKey(), GetBall, GetPaddle);

    PrefabsManager::Instance().SaveAsPrefab("PowerUp_Chaos", PowUP);
  }
}

void Game::initPowerUpSystem() {
  PowerUpSystem.TotalPowUp = 6;
  PowerUpSystem.Weights["PowerUp_Speed"] = 1.f / PowerUpSystem.TotalPowUp;
  PowerUpSystem.Weights["PowerUp_Sticky"] = 1.f / PowerUpSystem.TotalPowUp;
  PowerUpSystem.Weights["PowerUp_PassThrough"] = 1.f / PowerUpSystem.TotalPowUp;
  PowerUpSystem.Weights["PowerUp_Increase"] = 1.f / PowerUpSystem.TotalPowUp;
  PowerUpSystem.Weights["PowerUp_Confuse"] = 1.f / PowerUpSystem.TotalPowUp;
  PowerUpSystem.Weights["PowerUp_Chaos"] = 1.f / PowerUpSystem.TotalPowUp;
}
void Game::DoCollision() {

  for (auto &brick : Levels[curr_level].Bricks) {
    if (!brick.Destroyed) {
      if (BallObj->IsPassThrough == false) {
        if (auto colliInfo = BallObj->CheckCollision(brick); std::get<0>(colliInfo)) {
          brick.OnCollision(*BallObj, colliInfo);
          BallObj->OnCollision(brick, colliInfo);
          if (brick.IsSolid == false) {
            this->Levels[this->curr_level].DestroyableCount--;
            if (this->Levels[curr_level].IsCompleted()) {
              ++this->curr_level;
              Reset();
            }
            spawnPowerUps(brick);
          } else {
            PostProcessor::Instance().StartEffect(E_EffectType::Shake, 0.15f);
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
  for (auto &powerUp : PowerUpSystem.PowerUps) {
    if (!powerUp.Destroyed) {
      if (auto colliInfo = PaddleObj->CheckCollision(powerUp); std::get<0>(colliInfo)) {
        PaddleObj->OnCollision(powerUp, colliInfo);
        powerUp.OnCollision(*PaddleObj, colliInfo);
      }
    }
  }
}
void Game::Reset() {
  std::for_each(Levels.begin(), Levels.end(), [](GameLevel &lv) { lv.Reset(); });
  PaddleObj->Reset(this->Width, this->Height);
  BallObj->Reset(*PaddleObj);
  PostProcessor::Instance().EndEffect(E_EffectType::All);
  PowerUpSystem.Reset();
}

void Game::spawnPowerUps(GameObject &block) {
  // spawn chance will be 1 / chance
  auto ShouldSpawn = [](unsigned int chance) -> bool {
    unsigned int random = rand() % chance;
    return random == 0u;
  };

  if (ShouldSpawn(3)) {
    PowerUpSystem.RandomGenerate(block.Position);
  }
}

void Game::Fail() {
  Reset();
}
