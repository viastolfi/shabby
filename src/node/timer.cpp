#include "node/timer.h"

namespace Shabby::Node {

//TODO: add an abstraction layer so INode doees not need position
Timer::Timer(float duration)
  :INode(Vector2{-1, -1}), _duration(duration), _time_left(duration)
{}

void Timer::Update(float dt) 
{
  if (!_running)
    return;

  _time_left -= dt;
  if (_time_left <= 0.0f) {
    timeout.emit();
    if (!_run_once)
      Start();
  }

  INode::Update(dt);
}

void Timer::Start() 
{
  _running = true;
  _time_left = _duration;
}

} // namespace Shabby::Node
