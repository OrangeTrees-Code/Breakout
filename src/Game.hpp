#pragma once

// gl loader
#include "Camera.hpp"
#include "EGameState.hpp"
#include <array>
#include <glad/glad.h>
// opengl
#include "Ball.hpp"
#include "GameLevel.hpp"
#include "Menu.hpp"
#include "Paddle.hpp"
#include "PowerUpModule.hpp"
#include <GLFW/glfw3.h>
#include <memory>
class Game {

public:
  Game(GLuint width, GLuint height) : Width(width), Height(height), State(GameState::GAME_MENU) {};
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
  std::array<bool, 1024> KeysProcessed = {false};
  GLuint Width;
  GLuint Height;
  std::vector<GameLevel> Levels;
  int curr_level = 0;
  int ConqueredCount = 0;
  std::unique_ptr<Paddle> PaddleObj;
  std::unique_ptr<Ball> BallObj;
  // std::vector<PowerUp> PowerUps;
  PowerUpModule PowerUpSystem;
  int Lives = 3;
  Menu MenuPannel;

private:
  void initResourceManager();
  void initShader(Camera &mainCamera, Path &projroot);
  void initSprite(Path &projroot, int framebuffer_width, int framebuffer_height);
  void initLevel(Path &projroot);
  void initPaddle();
  void initBall();
  void initParticleSystem();
  void initPostProcessor(int framebuffer_width, int framebuffer_height);
  void initPrefabsManager();
  void initPowerUpSystem();
  void initAudioSystem(Path &projroot);
  void initTextRenderer(Path &projroot);
  void initMenu();
  void spawnPowerUps(GameObject &block);
};
