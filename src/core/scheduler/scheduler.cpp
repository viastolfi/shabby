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
  for (auto it = tasks.begin(); it != tasks.end(); ) {
    it->timer -= dt; 
    if (it->timer < 0.f) {
      it->task();  
      if (it->repeat) {
        it->timer = it->interval;
        ++it;
      } else {
        it = tasks.erase(it);
      }
    } else {
      ++it;
    }
  }  
}

} // namespace engine
