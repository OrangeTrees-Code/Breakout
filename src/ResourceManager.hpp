#pragma once

#include "Shader.tpp"
#include "Texture2D.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>

class ResourceManager {
  using Path = std::filesystem::path;

public:
  Path ProjRoot = Path(PROJECT_ROOT);

public:
  static ResourceManager &Instance() { return instance_; }

  Shader &LoadShader(const std::string &name, const Path &vertPath, const Path &fragPath);
  Shader &LoadShader(const std::string &name, const Path &vertPath, const Path &fragPath, const Path &geomPath);
  Shader &GetShader(const std::string &name);

  Texture2D &LoadTexture2D(const std::string &name, const Path &texturePath, bool isAlpha = false);
  Texture2D &GetTexture(const std::string &name);

  void Clear();

private:
  ResourceManager() = default;
  ~ResourceManager() { Clear(); }
  unsigned int initVertexShader(const char *vertexShaderSource);

  unsigned int initFragmentShader(const char *fragmentShaderSource);

  unsigned int initGeometryShader(const char *geometryShaderSource);

  unsigned int initShaderProgram(unsigned int vertexShader,
                                 unsigned int fragmentShader);

  unsigned int initShaderProgram(unsigned int vertexShader,
                                 unsigned int geometryShader,
                                 unsigned int fragmentShader);

private:
  static ResourceManager instance_;

  int success_;
  std::array<char, 512> infoLog_;
  std::unordered_map<std::string, Shader> shaders_;
  std::unordered_map<std::string, Texture2D> textures_2d_;
};
