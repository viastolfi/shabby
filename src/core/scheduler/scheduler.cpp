#include "core/scheduler/scheduler.h"

namespace engine {

void Scheduler::Every(float seconds, Task task)
{
  tasks.push_back(ScheduledTask{seconds, seconds, true, task}); 
}

void Scheduler::After(float seconds, Task task)
{
  tasks.push_back(ScheduledTask{seconds, 0.f, false, task});
}

void Scheduler::Update(float dt)
{
  for (auto& t : tasks) {
    t.timer -= dt; 
    if (t.timer < 0.f) {
      t.task();  
      if (t.repeat)
        t.timer = t.interval;
    }
  }  
}

} // namespace engine
