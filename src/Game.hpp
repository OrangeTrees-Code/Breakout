#pragma once

// gl loader
#include "Camera.hpp"
#include <array>
#include <glad/glad.h>
// opengl
#include "GameLevel.hpp"
#include "Player.hpp"
#include <GLFW/glfw3.h>
#include <memory>
enum class GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_Win,
};
enum class E_OutOfBound : unsigned int {
  Nope = 0u,
  Left = 1u,
  Right = 2u,
  Down = 4u,
  Up = 8u,
};

class Game {
  using OutOfBoundMask = unsigned int;

public:
  Game(GLuint width, GLuint height) : Width(width), Height(height), State(GameState::GAME_ACTIVE) {};
  ~Game() = default;
  void Init(Camera &mainCamera);
  void ProcessInput();
  void Update(GLfloat dt);
  void Render();

public:
  GameState State;
  std::array<bool, 1024> Keys = {false};
  GLuint Width;
  GLuint Height;
  std::vector<GameLevel> Levels;
  int curr_level = 0;
  std::unique_ptr<Player> PlayerObject;

private:
private:
  OutOfBoundMask isPlayerOutOfBound();
};
