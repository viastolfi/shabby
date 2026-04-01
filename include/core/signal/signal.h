#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <vector>

namespace Shabby::Core {

template<typename... Args>
class Signal {
public:
  using Callback = std::function<void(Args...)>;

  void connect(Callback cb) {
    callbacks.push_back(cb);
  }

  void emit(Args... args) {
    for (auto& cb : callbacks) {
      cb(args...);
    }
  }

private:
  std::vector<Callback> callbacks;
};

} // namespace Shabby::Core

#endif // SIGNAL_H
