#pragma once

// gl loader
#include "Camera.hpp"
#include <array>
#include <glad/glad.h>
// opengl
#include <GLFW/glfw3.h>

enum class GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_Win,
};

class Game {
public:
  Game(GLuint width, GLuint height) : Width(width), Height(height), State(GameState::GAME_ACTIVE) {};
  void Init(Camera &mainCamera);
  void ProcessInput(GLfloat dt) {};
  void Update(GLfloat dt) {};
  void Render();
  GameState State;
  std::array<bool, 1024> Keys = {false};
  GLuint Width;
  GLuint Height;
  ~Game() = default;
};
