#pragma once
// OpenGL loader
#include "IResource.ipp"
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
class ResourceManager;

class Shader : IResource {
  friend class ResourceManager;

private:
  unsigned int ID_;

  mutable std::unordered_map<std::string, int> uniformCache_;

public:
  Shader(Shader &&other) noexcept;
  Shader &operator=(Shader &&other) noexcept;

  ~Shader() { release(); }

  void Use() const;

  unsigned int Get_ID() const;

  template <typename T>
  void SetUniform(const std::string &name, const T &value, const int length = -1) const;

private:
  template <typename T>
  void SetUniformImpl(int location, const T &value) const;

  int GetUniformLocation(const std::string &name) const;

  void release() override;
  Shader(unsigned int id) : ID_(id), uniformCache_() {};
  Shader(const Shader &other) = delete;
  Shader &operator=(const Shader &other) = delete;
};

template <typename T>
void Shader::SetUniform(const std::string &name, const T &value, const int length) const {

  if constexpr (std::is_array_v<std::remove_cvref_t<T>>) {
    int location;
    for (int i = 0; i < length; ++i) {
      location = GetUniformLocation(name + "[" + std::to_string(i) + "]");
      SetUniformImpl(location, value[i]);
    }
  } else {
    int location = GetUniformLocation(name);
    SetUniformImpl(location, value);
  }
}

template <typename T>
void Shader::SetUniformImpl(int location, const T &value) const {
  static_assert(sizeof(T) == 0, "Unsupported uniform type!");
}

template <>
inline void Shader::SetUniformImpl<int>(int location, const int &value) const {
  glUniform1i(location, value);
}

template <>
inline void Shader::SetUniformImpl<float>(int location, const float &value) const {
  glUniform1f(location, value);
}

template <>
inline void Shader::SetUniformImpl<bool>(int location, const bool &value) const {
  glUniform1i(location, (int)value);
}

template <>
inline void Shader::SetUniformImpl<glm::vec3>(int location, const glm::vec3 &value) const {
  glUniform3fv(location, 1, glm::value_ptr(value));
}

template <>
inline void Shader::SetUniformImpl<glm::vec4>(int location, const glm::vec4 &value) const {
  glUniform4fv(location, 1, glm::value_ptr(value));
}

template <>
inline void Shader::SetUniformImpl<glm::vec2>(int location, const glm::vec2 &value) const {
  glUniform2fv(location, 1, glm::value_ptr(value));
}

template <>
inline void Shader::SetUniformImpl<glm::mat4>(int location, const glm::mat4 &value) const {
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
inline void Shader::SetUniformImpl<glm::mat3>(int location, const glm::mat3 &value) const {
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
