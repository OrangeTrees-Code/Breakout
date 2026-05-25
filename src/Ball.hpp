#include "DynamicGOBase.hpp"
#include "Paddle.hpp"
class Ball : public DynamicGOBase, public IManipulable {
public:
  float Radius;
  bool IsStuck;

public:
  Ball() : DynamicGOBase() {};
  Ball(float radius, bool isStuck, const glm::vec2 &position, const glm::vec2 &size, Texture2D &sprite, const glm::vec3 &color = glm::vec3(1.f), const glm::vec2 &velocity = glm::vec2(0.f, 0.f)) : DynamicGOBase(position, size, sprite, color, velocity), Radius(radius), IsStuck(isStuck) {};

  void Move(float deltaTime, int width, int height) override;
  void ManipulableHandle(std::array<bool, 1024> &Keys) override;
  void SetLaunchPaddle(Paddle &paddle) { launchPaddle_ = &paddle; }

private:
  Paddle *launchPaddle_ = nullptr;
};