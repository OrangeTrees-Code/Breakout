#include "Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
  // return Global::Instance().MyLookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
  return glm::perspective(glm::radians(FOV),
                          (float)ViewWidth / (float)ViewHeight, 0.1f, 100.0f);
}

void Camera::UpdateCameraVec() {
  glm::vec3 front;

  const float pitch = glm::radians(Pitch);
  const float yaw = glm::radians(Yaw);
  front.x = std::cos(pitch) * std::cos(yaw);
  front.y = std::sin(pitch);
  front.z = std::cos(pitch) * std::sin(yaw);
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, glm::vec3(0.f,1.f,0.f)));
  Up = glm::normalize(glm::cross(Right, Front));
}