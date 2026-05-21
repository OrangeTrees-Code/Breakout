#pragma once

// gl loader
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
  Game(GLuint width, GLuint height) : Width(width), Height(height), State(GameState::GAME_ACTIVE) { Init(); };
  void Init() {};
  void ProcessInput(GLfloat dt) {};
  void Update(GLfloat dt) {};
  void Render() {};
  GameState State;
  GLboolean Keys[1024] = {false};
  GLuint Width;
  GLuint Height;
  ~Game() = default;
};
