#pragma once

#include "Global.hpp"
#include "InitKey.hpp"
#include "Shader.tpp"
#include "Texture2D.hpp"
#include <string>
#include <unordered_map>
struct Font {
  struct Character {
    Texture2D *Texture;  // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    signed long Advance; // Horizontal offset to advance to next glyph
  };
  std::unordered_map<char, Character> Characters;
};

class ResourceManager {
public:
  Path ProjRoot = Path(PROJECT_ROOT);

public:
  static ResourceManager &Instance();
  static void Init(InitKey);
  ~ResourceManager() { Clear(); }

  ResourceManager(const ResourceManager &other) = delete;
  ResourceManager &operator=(const ResourceManager &other) = delete;

  Shader &LoadShader(const std::string &name, const Path &vertPath, const Path &fragPath);
  Shader &LoadShader(const std::string &name, const Path &vertPath, const Path &fragPath, const Path &geomPath);
  Shader &GetShader(const std::string &name);

  Texture2D &LoadTexture2D(const std::string &name, const Path &texturePath, bool isAlpha = false);
  Texture2D &LoadEmptyTextureBuffer(const std::string &name, int width, int height, bool isAlpha = false);
  Texture2D &GetTexture(const std::string &name);

  void LoadFonts(const Path &path, unsigned int fontSize);
  Font& GetFont(const std::string name);

  void Clear();

private:
  ResourceManager() = default;
  unsigned int initVertexShader(const char *vertexShaderSource);

  unsigned int initFragmentShader(const char *fragmentShaderSource);

  unsigned int initGeometryShader(const char *geometryShaderSource);

  unsigned int initShaderProgram(unsigned int vertexShader,
                                 unsigned int fragmentShader);

  unsigned int initShaderProgram(unsigned int vertexShader,
                                 unsigned int geometryShader,
                                 unsigned int fragmentShader);

private:
  static std::unique_ptr<ResourceManager> instance_;

  int success_;
  std::array<char, 512> infoLog_;
  std::unordered_map<std::string, Shader> shaders_;
  std::unordered_map<std::string, Texture2D> textures_2d_;
  std::unordered_map<std::string, Font> fonts_;
};
