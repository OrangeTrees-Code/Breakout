#pragma once

#include "Shader.tpp"
#include "Texture2D.hpp"
class PostProcessor {
public:
  PostProcessor(unsigned int width, unsigned int height, Shader &sdr, Texture2D &textureBuffer) : FrameBuffer_Width(width), FrameBuffer_Height(height), shader_(&sdr), sceneTexture_(&textureBuffer) { init(); }
  ~PostProcessor();
  Shader &GetShader() { return *shader_; }
  void SetShader(Shader &sdr) { shader_ = &sdr; }
  void BeginRender();
  void EndRender();
  void Render(float time);

public:
  unsigned int FrameBuffer_Width;
  unsigned int FrameBuffer_Height;
  bool Chaos = false;
  bool Confuse = false;
  bool Shake = false;

private:
  void init();
  void initVAO();

private:
  Shader *shader_;
  Texture2D *sceneTexture_;
  unsigned int vao_;
  unsigned int msfbo;
  unsigned int fbo_;
  unsigned int rbo_;
};
