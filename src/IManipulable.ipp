#include <array>
class IManipulable {
public:
  virtual void ManipulableHandle(std::array<bool, 1024> &Keys) = 0;
  virtual ~IManipulable() = default;
};