#pragma once

// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
enum class Direction {
  Forward,
  Backward,
  Left,
  Right,
  Up,
  Down,
};

class Camera {
  friend void processInput(Camera &camera);

public:
  float MoveSpeed = 2.0f;
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  int ViewWidth;
  int ViewHeight;
  float Yaw;
  float Pitch;
  float FOV;

  Camera(int width = 800, int height = 600,
         glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 Direction = glm::vec3(0.0f, 0.0f, -1.0f),
         glm::vec3 UpDirection = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = -90.0f, float pitch = 0.0f, float fov = 45.0f)
      : Position(Position), Front(Direction), Up(UpDirection),
        Right(glm::cross(Front, Up)), Yaw(yaw), Pitch(pitch), FOV(fov),
        ViewWidth(width), ViewHeight(height) {}

  glm::mat4 GetViewMatrix();

  glm::mat4 GetProjectionMatrix_Persp(float zNear = 0.1f, float zFar = 100.f);

  glm::mat4 GetProjectionMatrix_Ortho(float zNear = -1.f, float zFar = 1.f);
};