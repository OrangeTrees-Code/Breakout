#include <functional>
template <typename... Args>
class Event {
public:
  using Callback = std::function<void(Args...)>;

  void Subscribe(Callback callback) { callbacks_.push_back(callback); }

  void Broadcast(Args... args) {
    for (auto &callback : callbacks_)
      callback(args...);
  }

private:
  std::vector<Callback> callbacks_;
};