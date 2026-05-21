#pragma once

#include "Camera.hpp"
#include "Game.hpp"
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <memory>
#include <stdexcept>

class Window {
public:
  float deltaTime = 0.0f;
  const int Width = 0;
  const int Height = 0;
  Camera &MainCamera;
  Game& GameProgram;

public:
  Window(int width, int height, bool isTransparent, Camera &mainCamera,Game& game,
         float cursorSensitivity = 0.005f, float scrollSensitivity = 1.0f)
      : Width(width), Height(height), window_ptr_(WindowInit(isTransparent)),
        MainCamera(mainCamera),GameProgram(game) {
    if (window_ptr_.get() == NULL) {
      throw std::runtime_error("Window Create Fail!");
    }
  };
  GLFWwindow *Get_Window_Ptr() { return window_ptr_.get(); }
  void ResetViewPort(int width, int height);
  void ResetViewPort();

  void UpdateDeltaTime();
  int GetFrameBufferWidth() const { return frameBufferWidth_; }
  int GetFrameBufferHeight() const { return frameBufferHeight_; }

private:
  struct GLFWwindowDeleter {
    void operator()(GLFWwindow *w) const {
      if (w)
        glfwDestroyWindow(w);
    }
  };
  using WindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>;

private:
  float currentFrame = 0.0f;
  float lastFrame = 0.0f;
  WindowPtr window_ptr_;

  int frameBufferWidth_;
  int frameBufferHeight_;

private:
  GLFWwindow *WindowInit(bool isTransparent);

  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
};