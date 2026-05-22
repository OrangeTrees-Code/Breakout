#include "Game.hpp"
#include "Camera.hpp"
#include "RenderSprite.hpp"
#include "ResourceManager.hpp"

void Game::Init(Camera &mainCamera) {
  auto &projroot = ResourceManager::Instance().ProjRoot;
  auto &shader = ResourceManager::Instance().LoadShader("default_sprite_shader", projroot / "src/shader/sprite.vert", projroot / "src/shader/sprite.frag");
  SpriteRender::Instance().SetShader(shader);
  shader.Use();
  shader.SetUniform("projection", mainCamera.GetProjectionMatrix_Ortho());
  shader.SetUniform("image", 0);
  ResourceManager::Instance().LoadTexture2D("face", projroot / "res/texture/awesomeface.png", true);
}

void Game::Render() {
  SpriteRender::Instance().DrawSprite(ResourceManager::Instance().GetTexture("face"), glm::vec2(0.f, 0.f), glm::vec2(300.f, 400.f), 0.f);
}
