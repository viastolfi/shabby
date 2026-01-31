#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <functional>

namespace engine {

struct ScheduledTask;

class Scheduler {
public:
  using Task = std::function<void()>;
  
  void Every(float seconds, Task task);
  void After(float seconds, Task task);
  void Update(float dt);
private:
  std::vector<ScheduledTask> tasks;
};

struct ScheduledTask {
  float timer;
  float interval;
  bool repeat;
  Scheduler::Task task;
};


} // namespace engine

#endif // SCHEDULER_H
