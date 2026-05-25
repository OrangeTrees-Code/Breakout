#include "ResourceManager.hpp"
#include "Shader.tpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

ResourceManager ResourceManager::instance_;

namespace {
std::string ReadTextFile(const std::filesystem::path &path) {
  std::ifstream file;
  file.exceptions(std::ifstream::badbit | std::ifstream::failbit);

  try {
    file.open(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  } catch (const std::ifstream::failure &) {
    throw std::runtime_error("ERROR::RESOURCE_MANAGER::FILE_NOT_SUCCESSFULLY_READ: " +
                             path.string());
  }
}
} // namespace

void ResourceManager::Clear() {
  shaders_.clear();
  textures_2d_.clear();
}

Shader &ResourceManager::GetShader(const std::string &name) {
  if (!shaders_.contains(name)) {
    throw std::runtime_error("ERROR::RESOURCEMANAGER::GetShader: " + name + " doesn't exist!");
  }
  return shaders_.at(name);
}

Texture2D &ResourceManager::GetTexture(const std::string &name) {
  if (!textures_2d_.contains(name)) {
    throw std::runtime_error("ERROR::RESOURCEMANAGER::GetTexture: " + name + " doesn't exist!");
  }
  return textures_2d_.at(name);
}

unsigned int ResourceManager::initVertexShader(const char *vertexShaderSource) {
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_);

  if (!success_) {
    glGetShaderInfoLog(vertexShader, sizeof(infoLog_.data()), NULL, infoLog_.data());
    throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED: " + std::string(infoLog_.data()));
  }

  return vertexShader;
}

unsigned int ResourceManager::initFragmentShader(const char *fragmentShaderSource) {
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_);

  if (!success_) {
    glGetShaderInfoLog(fragmentShader, sizeof(infoLog_), NULL, infoLog_.data());
    throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " + std::string(infoLog_.data()));
  }

  return fragmentShader;
}

unsigned int ResourceManager::initGeometryShader(const char *geometryShaderSource) {
  unsigned int geometryShader;
  geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
  glCompileShader(geometryShader);

  glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success_);

  if (!success_) {
    glGetShaderInfoLog(geometryShader, sizeof(infoLog_.data()), NULL, infoLog_.data());
    throw std::runtime_error("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED: " + std::string(infoLog_.data()));
  }

  return geometryShader;
}

unsigned int ResourceManager::initShaderProgram(unsigned int vertexShader,
                                                unsigned int fragmentShader) {
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_);
  if (!success_) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog_.data());
    throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED: " + std::string(infoLog_.data()));
  }

  return shaderProgram;
}

unsigned int ResourceManager::initShaderProgram(unsigned int vertexShader,
                                                unsigned int geometryShader,
                                                unsigned int fragmentShader) {
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, geometryShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_);
  if (!success_) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog_.data());
    throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED: " + std::string(infoLog_.data()));
  }

  return shaderProgram;
}

Shader &ResourceManager::LoadShader(const std::string &name,
                                    const Path &vertPath,
                                    const Path &fragPath) {
  if (shaders_.contains(name)) {
    throw std::runtime_error("ERROR::RESOURCEMANAGER::LoadShader: " + name + " already exist!");
  }

  std::string vertSrc = ReadTextFile(vertPath);
  std::string fragSrc = ReadTextFile(fragPath);

  unsigned int vertexShader = 0;
  unsigned int fragmentShader = 0;
  unsigned int ID = 0;

  try {
    vertexShader = this->initVertexShader(vertSrc.c_str());
    fragmentShader = this->initFragmentShader(fragSrc.c_str());
    ID = this->initShaderProgram(vertexShader, fragmentShader);
  } catch (...) {
    if (vertexShader != 0) {
      glDeleteShader(vertexShader);
    }
    if (fragmentShader != 0) {
      glDeleteShader(fragmentShader);
    }
    throw;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  auto [it, inserted] = shaders_.emplace(name, Shader(ID));
  return it->second;
}

Shader &ResourceManager::LoadShader(const std::string &name,
                                    const Path &vertPath,
                                    const Path &fragPath,
                                    const Path &geomPath) {
  if (shaders_.contains(name)) {
    throw std::runtime_error("ERROR::RESOURCEMANAGER::LoadShader: " + name + " already exist!");
  }

  std::string vertSrc = ReadTextFile(vertPath);
  std::string geomSrc = ReadTextFile(geomPath);
  std::string fragSrc = ReadTextFile(fragPath);

  unsigned int vertexShader = 0;
  unsigned int geometryShader = 0;
  unsigned int fragmentShader = 0;
  unsigned int ID = 0;

  try {
    vertexShader = this->initVertexShader(vertSrc.c_str());
    geometryShader = this->initGeometryShader(geomSrc.c_str());
    fragmentShader = this->initFragmentShader(fragSrc.c_str());
    ID = this->initShaderProgram(vertexShader, geometryShader, fragmentShader);
  } catch (...) {
    if (vertexShader != 0) {
      glDeleteShader(vertexShader);
    }
    if (geometryShader != 0) {
      glDeleteShader(geometryShader);
    }
    if (fragmentShader != 0) {
      glDeleteShader(fragmentShader);
    }
    throw;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  auto [it, inserted] = shaders_.emplace(name, Shader(ID));
  return it->second;
}

Texture2D &ResourceManager::LoadTexture2D(const std::string &name,
                                          const Path &texturePath,
                                          bool isAlpha) {
  if (textures_2d_.contains(name)) {
    throw std::runtime_error("ERROR::RESOURCEMANAGER::LoadTexture2D: " + name + " already exist!");
  }

  Texture2D texture;
  if (isAlpha) {
    texture.internal_format_ = GL_RGBA;
    texture.image_format_ = GL_RGBA;
  }

  int width;
  int height;
  int nrChannels;
  int desiredChannels = isAlpha ? STBI_rgb_alpha : STBI_rgb;
  // stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(texturePath.string().c_str(), &width, &height, &nrChannels,
                desiredChannels);

  if (data == nullptr) {
    throw std::runtime_error("ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ: " +
                             texturePath.string());
  }

  texture.Generate(static_cast<GLuint>(width), static_cast<GLuint>(height),
                   data);
  stbi_image_free(data);

  auto [it, inserted] = textures_2d_.emplace(name, std::move(texture));
  return it->second;
}
