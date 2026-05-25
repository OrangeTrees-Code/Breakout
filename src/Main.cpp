
#include "Camera.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <endian.h>
int main() {
  Game breakout(800, 600);

  Camera mainCamera(breakout.Width, breakout.Height);

  Window window(breakout.Width, breakout.Height, true, mainCamera, breakout);

  breakout.Init(mainCamera);

  while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
    window.UpdateDeltaTime();
    glfwPollEvents();

    breakout.ProcessInput();

    breakout.Update(window.deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    breakout.Render();

    glfwSwapBuffers(window.Get_Window_Ptr());
  }

  ResourceManager::Instance().Clear();

  glfwTerminate();
  return 0;
}
