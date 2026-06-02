#include "PostProcessor.hpp"
#include "Primitive.hpp"

#include <glad/glad.h>
#include <stdexcept>

std::unique_ptr<PostProcessor> PostProcessor::instance_ = nullptr;

PostProcessor &PostProcessor::Instance() {
  if (!instance_) {
    throw std::runtime_error("PostProcessor has not been initialized");
  }
  return *instance_;
}
void PostProcessor::Init(InitKey, unsigned int width, unsigned int height, Shader &sdr, Texture2D &textureBuffer) {
  instance_ = std::unique_ptr<PostProcessor>(new PostProcessor(width, height, sdr, textureBuffer));
}
PostProcessor::~PostProcessor() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteRenderbuffers(1, &rbo_);
  glDeleteFramebuffers(1, &msfbo);
  glDeleteFramebuffers(1, &fbo_);
}
void PostProcessor::init() {
  glGenFramebuffers(1, &fbo_);
  glGenFramebuffers(1, &msfbo);
  glGenRenderbuffers(1, &rbo_);

  glBindFramebuffer(GL_FRAMEBUFFER, msfbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, FrameBuffer_Width, FrameBuffer_Height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("ERROR::POSTPROCESSOR: Failed to initialize MSFBO");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->sceneTexture_->Get_ID(), 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("ERROR::POSTPROCESSOR: Failed to initialize FBO");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  initVAO();
  this->shader_->Use();
  shader_->SetUniform("scene", 2);
  GLfloat offset = 1.0f / 300.0f;
  glm::vec2 offsets[9] = {
      {-offset, offset},  // top-left
      {0.0f, offset},     // top-center
      {offset, offset},   // top-right
      {-offset, 0.0f},    // center-left
      {0.0f, 0.0f},       // center-center
      {offset, 0.0f},     // center - right
      {-offset, -offset}, // bottom-left
      {0.0f, -offset},    // bottom-center
      {offset, -offset}   // bottom-right
  };
  shader_->SetUniform("offset", offsets, 9);
  int edge_kernel[9] = {
      -1, -1, -1,
      -1, 8, -1,
      -1, -1, -1};
  shader_->SetUniform("edge_kernel", edge_kernel, 9);
  float blur_kernel[9] = {
      1.0 / 16, 2.0 / 16, 1.0 / 16,
      2.0 / 16, 4.0 / 16, 2.0 / 16,
      1.0 / 16, 2.0 / 16, 1.0 / 16};

  shader_->SetUniform("blur_kernel", blur_kernel, 9);
}

void PostProcessor::initVAO() {
  unsigned int VBO;

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &VBO);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Quad::screenVertices), Quad::screenVertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void PostProcessor::BeginRender() {
  glBindFramebuffer(GL_FRAMEBUFFER, msfbo);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, msfbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
  glBlitFramebuffer(0, 0, FrameBuffer_Width, FrameBuffer_Height, 0, 0, FrameBuffer_Width, FrameBuffer_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(float time) {
  shader_->Use();
  shader_->SetUniform("time", time);
  this->shader_->SetUniform("confuse", this->Confuse);
  this->shader_->SetUniform("chaos", this->Chaos);
  this->shader_->SetUniform("shake", this->Shake);
  glActiveTexture(GL_TEXTURE2);
  sceneTexture_->Bind();
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void PostProcessor::Update(float deltaTime) {
  if (shakeTime_ > 0.f) {
    shakeTime_ -= deltaTime;
    if (shakeTime_ < 0.f) {
      shakeTime_ = 0.f;
      Shake = false;
    }
  }
  if (chaosTime_ > 0.f) {
    chaosTime_ -= deltaTime;
    if (chaosTime_ < 0.f) {
      chaosTime_ = 0.f;
      Chaos = false;
    }
  }
  if (confuseTime_ > 0.f) {
    confuseTime_ -= deltaTime;
    if (confuseTime_ < 0.f) {
      confuseTime_ = 0.f;
      Confuse = false;
    }
  }
}

void PostProcessor::StartEffect(E_EffectType type, float time) {
  if ((EffectMaskType)type & (EffectMaskType(E_EffectType::Chaos))) {
    Chaos = true;
    chaosTime_ = time;
  }
  if ((EffectMaskType)type & (EffectMaskType(E_EffectType::Confuse))) {
    Confuse = true;
    confuseTime_ = time;
  }
  if ((EffectMaskType)type & (EffectMaskType(E_EffectType::Shake))) {
    Shake = true;
    shakeTime_ = time;
  }
}

void PostProcessor::EndEffect(E_EffectType type) {
  if ((EffectMaskType)type & (EffectMaskType(E_EffectType::Chaos))) {
    Chaos = false;
    chaosTime_ = 0.f;
  }
  if ((EffectMaskType)type & (EffectMaskType(E_EffectType::Confuse))) {
    Confuse = false;
    confuseTime_ = 0.f;
  }
  if ((EffectMaskType)type & (EffectMaskType(E_EffectType::Shake))) {
    Shake = false;
    shakeTime_ = 0.f;
  }
}