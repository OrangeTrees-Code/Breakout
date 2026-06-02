#pragma once

#include "GameObject.hpp"
#include "ParticleObject.hpp"
#include "Shader.tpp"
#include "Texture2D.hpp"
#include "InitKey.hpp"
#include <memory>
#include <stack>
#include <vector>
class ParticleGenerator {

public:
  static ParticleGenerator &Instance();
  static void Init(InitKey, int totalNum, int newNum, const glm::vec2 &offset, Shader &sdr, Texture2D &tex);
  ParticleGenerator(const ParticleGenerator &other) = delete;
  ParticleGenerator &operator=(const ParticleGenerator &other) = delete;
  ~ParticleGenerator() { glDeleteVertexArrays(1, &vao_); }
  void Update(float deltaTime, GameObject &object);
  void Render();
  Shader &GetShader() { return *shader; }
  void SetShader(Shader &sha) { shader = &sha; }
  Texture2D &GetSprite() { return *sprite; }
  void SetSprite(Texture2D &spr) { sprite = &spr; }

public:
  int TotalNum_Particles;
  int NewNum_Particle;
  glm::vec2 Offset;

private:
  ParticleGenerator(int totalNum, int newNum, const glm::vec2 &offset, Shader &sdr, Texture2D &tex) : TotalNum_Particles(totalNum), NewNum_Particle(newNum), Offset(offset), shader(&sdr), sprite(&tex) { init(); }
  int getFirstUnuesdIndex();
  void respawnParticle(ParticleObject &particle, GameObject &object);
  void init();
  void initVAO();

private:
  static std::unique_ptr<ParticleGenerator> instance_;
  std::vector<ParticleObject> particles_;
  std::stack<int> unuesdIndex_;
  unsigned int vao_;
  Shader *shader = nullptr;
  Texture2D *sprite = nullptr;
};