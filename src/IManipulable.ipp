#pragma once

#include <array>
class IManipulable {
public:
  virtual void ManipulableHandle(std::array<bool, 1024> &Keys, std::array<bool, 1024> &KeysProcessed) = 0;
  virtual ~IManipulable() = default;
};