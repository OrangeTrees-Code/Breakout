#pragma once

// gl loader
#include "Camera.hpp"
#include <array>
#include <glad/glad.h>
// opengl
#include "Ball.hpp"
#include "GameLevel.hpp"
#include "Paddle.hpp"
#include "PowerUpModule.hpp"
#include <GLFW/glfw3.h>
#include <memory>
enum class GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_Win,
};

class Game {

public:
  Game(GLuint width, GLuint height) : Width(width), Height(height), State(GameState::GAME_ACTIVE) {};
  ~Game() = default;
  void Init(Camera &mainCamera, int framebuffer_width, int framebuffer_height);
  void ProcessInput();
  void Update(GLfloat dt);
  void Render();
  void DoCollision();
  void Fail();
  void Reset();

public:
  GameState State;
  std::array<bool, 1024> Keys = {false};
  GLuint Width;
  GLuint Height;
  std::vector<GameLevel> Levels;
  int curr_level = 0;
  std::unique_ptr<Paddle> PaddleObj;
  std::unique_ptr<Ball> BallObj;
  // std::vector<PowerUp> PowerUps;
  PowerUpModule PowerUpSystem;

private:
  void initShader(Camera &mainCamera, Path &projroot);
  void initSprite(Path &projroot, int framebuffer_width, int framebuffer_height);
  void initLevel(Path &projroot);
  void initPaddle();
  void initBall();
  void initParticleSystem();
  void initPostProcessor(int framebuffer_width, int framebuffer_height);
  void initPrefabsManager();
  void initPowerUpSystem();

  void spawnPowerUps(GameObject &block);
};
