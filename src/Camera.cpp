#include "Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
  // return Global::Instance().MyLookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix_Persp(float zNear, float zFar) {
  return glm::perspective(glm::radians(FOV),
                          (float)ViewWidth / (float)ViewHeight, zNear, zFar);
}

glm::mat4 Camera::GetProjectionMatrix_Ortho(float zNear, float zFar) {
  return glm::ortho(0.f, (float)ViewWidth, (float)ViewHeight, 0.f, zNear, zFar);
}
