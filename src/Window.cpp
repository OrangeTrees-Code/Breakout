#include "Window.hpp"
#include "Camera.hpp"
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

GLFWwindow *Window::WindowInit(bool isTransparent) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GL_MULTISAMPLE, 64);
  if (isTransparent == true) {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  }

  GLFWwindow *window = glfwCreateWindow(Width, Height, "Suki", NULL, NULL);
  glfwWindowHintString(GLFW_WAYLAND_APP_ID, "Suki");
  if (window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    return NULL;
  }
  glfwMakeContextCurrent(window);

  // glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  // glViewport(0, 0, Width, Height);
  int fbWidth, fbHeight;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  this->frameBufferWidth_ = fbWidth;
  this->frameBufferHeight_ = fbHeight;

  glfwSetWindowUserPointer(window, this);
  // glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    self->key_callback(window, key, scancode, action, mode);
  });
  return window;
}

void Window::UpdateDeltaTime() {
  currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      GameProgram.Keys[key] = GL_TRUE;
    else if (action == GLFW_RELEASE)
      GameProgram.Keys[key] = GL_FALSE;
  }
}

void Window::ResetViewPort(int width, int height) {
  glViewport(0, 0, width, height);
}
void Window::ResetViewPort() {
  this->ResetViewPort(frameBufferWidth_, frameBufferHeight_);
}