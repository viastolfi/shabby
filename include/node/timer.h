#ifndef TIMER_H
#define TIMER_H

#include "node/inode.h"
#include "core/signal/signal.h"

namespace Shabby::Node {

class Timer final : public INode {
public:
  Core::Signal<> timeout;

  Timer(float duration);
  ~Timer() = default;

  void Update(float dt) override;
  void Start();

  bool _run_once = false;
private:
  float _duration;
  float _time_left;
  bool _running = false;
};

} // namespace Shabby::Node

#endif // TIMER_H
