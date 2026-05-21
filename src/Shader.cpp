#include "Shader.tpp"
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>


void Shader::Use() const { glUseProgram(Get_ID()); }

unsigned int Shader::Get_ID() const { return ID_; }

int Shader::GetUniformLocation(const std::string &name) const {
  if (uniformCache_.contains(name)) {
    return uniformCache_[name];
  }

  int location = glGetUniformLocation(this->Get_ID(), name.c_str());
  uniformCache_[name] = location;
  return location;
}

void Shader::release()  {
  if(ID_ != 0) {
    glDeleteProgram(ID_);
  }
  ID_= 0;
}

Shader::Shader(Shader &&other) noexcept: ID_(other.ID_), uniformCache_(std::move(other.uniformCache_)) {
  other.ID_ = 0;
}
Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    release();
    ID_ = other.ID_;
    uniformCache_ = std::move(other.uniformCache_);
    other.ID_ = 0;
  }
  return *this;
}
