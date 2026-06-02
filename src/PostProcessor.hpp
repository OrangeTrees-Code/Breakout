#pragma once

#include "InitKey.hpp"
#include "Shader.tpp"
#include "Texture2D.hpp"
#include <memory>

enum class E_EffectType : unsigned int {
  Chaos = 1u,
  Confuse = 2u,
  Shake = 4u,
  All = 7u
};
class PostProcessor {
  using EffectMaskType = unsigned int;

public:
  static PostProcessor &Instance();

  static void Init(InitKey, unsigned int width, unsigned int height, Shader &sdr, Texture2D &textureBuffer);

  ~PostProcessor();
  Shader &GetShader() { return *shader_; }
  void SetShader(Shader &sdr) { shader_ = &sdr; }
  void BeginRender();
  void EndRender();
  void Render(float time);
  void Update(float deltaTime);
  void StartEffect(E_EffectType type, float time);
  void EndEffect(E_EffectType type);

public:
  unsigned int FrameBuffer_Width;
  unsigned int FrameBuffer_Height;

  bool Shake = false;
  bool Chaos = false;
  bool Confuse = false;

private:
  void init();
  void initVAO();
  PostProcessor(unsigned int width, unsigned int height, Shader &sdr, Texture2D &textureBuffer) : FrameBuffer_Width(width), FrameBuffer_Height(height), shader_(&sdr), sceneTexture_(&textureBuffer) { init(); }

private:
  static std::unique_ptr<PostProcessor> instance_;

  Shader *shader_;
  Texture2D *sceneTexture_;
  unsigned int vao_;
  unsigned int msfbo;
  unsigned int fbo_;
  unsigned int rbo_;

  float shakeTime_ = 0.f;
  float chaosTime_ = 0.f;
  float confuseTime_ = 0.f;
};
