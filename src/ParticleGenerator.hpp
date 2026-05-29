#pragma once

#include "GameObject.hpp"
#include "ParticleObject.hpp"
#include "Shader.tpp"
#include "Texture2D.hpp"
#include <stack>
#include <vector>
class ParticleGenertor {
public:
  ParticleGenertor(int totalNum, int newNum, const glm::vec2 &offset, Shader &sdr, Texture2D &tex) : TotalNum_Particles(totalNum), NewNum_Particle(newNum), Offset(offset), shader(&sdr), sprite(&tex) { init(); }
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
  int getFirstUnuesdIndex();
  void respawnParticle(ParticleObject &particle, GameObject &object);
  void init();

private:
  std::vector<ParticleObject> particles_;
  std::stack<int> unuesdIndex_;
  unsigned int vao_;
  Shader *shader = nullptr;
  Texture2D *sprite = nullptr;
};