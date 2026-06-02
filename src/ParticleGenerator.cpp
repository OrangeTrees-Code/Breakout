#include "ParticleGenerator.hpp"
#include "DynamicGOBase.hpp"
#include "GameObject.hpp"
#include "Primitive.hpp"
#include "Shader.tpp"
#include "Texture2D.hpp"
#include <cstdlib>
#include <ranges>
#include <stdexcept>

std::unique_ptr<ParticleGenerator> ParticleGenerator::instance_ = nullptr;


ParticleGenerator &ParticleGenerator::Instance() {
  if (!instance_) {
    throw std::runtime_error("ParticleGenerator has not been initialized");
  }
  return *instance_;
}

void ParticleGenerator::Init(InitKey, int totalNum, int newNum, const glm::vec2 &offset, Shader &sdr, Texture2D &tex) {
  instance_ = std::unique_ptr<ParticleGenerator>(new ParticleGenerator(totalNum,newNum,offset,sdr,tex));
}

void ParticleGenerator::init() {
  particles_.resize(TotalNum_Particles);
  unuesdIndex_.push_range(std::views::iota(0, TotalNum_Particles) | std::views::reverse);

  unsigned int VBO;
  glGenVertexArrays(1, &this->vao_);
  glGenBuffers(1, &VBO);
  glBindVertexArray(this->vao_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Quad::unitVertices), Quad::unitVertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

int ParticleGenerator::getFirstUnuesdIndex() {
  if (unuesdIndex_.empty()) {
    return -1;
  }
  return unuesdIndex_.top();
  unuesdIndex_.pop();
}

void ParticleGenerator::respawnParticle(ParticleObject &particle, GameObject &object) {
  float random = ((rand() % 100) - 50) / 10.f;
  float rColor = 0.5f + ((rand() % 100) / 100.f);
  particle.Position = object.Position + random + Offset;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.f);
  particle.Life = 1.f;
  try {
    auto dynamic = dynamic_cast<DynamicGOBase &>(object);
    particle.Velocity = -1.f * dynamic.Velocity * 0.1f;
  } catch (...) {
    particle.Velocity = glm::vec2(0.f);
  }
};

void ParticleGenerator::Render() {
  if (shader == nullptr || sprite == nullptr) {
    throw std::runtime_error("Generator is not complete!");
  }

  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  this->shader->Use();
  for (auto &particle : particles_) {
    if (particle.Life == 0) {
      continue;
    }
    this->shader->SetUniform("offset", particle.Position);
    this->shader->SetUniform("color", particle.Color);
    glActiveTexture(GL_TEXTURE1);
    this->sprite->Bind();
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Update(float deltaTime, GameObject &object) {
  for (int i = 0; i < NewNum_Particle; ++i) {
    if (int unesed = getFirstUnuesdIndex(); unesed != -1) {
      respawnParticle(particles_[unesed], object);
    } else {
      break;
    }
  }

  for (int i = 0; i < particles_.size(); ++i) {
    ParticleObject &particle = particles_[i];
    particle.Life = std::max(0.f, particle.Life - deltaTime);
    if (particle.Life > 0.f) {
      particle.Position += particle.Velocity * deltaTime;
      particle.Color.a -= 2.5f * deltaTime;
    } else {
      unuesdIndex_.push(i);
    }
  }
}
