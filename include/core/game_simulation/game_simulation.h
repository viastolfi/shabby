#ifndef GAME_SIMULATION_H
#define GAME_SIMULATION_H

#include "core/sprite/sprite.h"
#include "scene/scene.h"
#include "core/scheduler/scheduler.h"
#include <memory>
#include <functional>
#include <random>

namespace engine {

class GameSimulation {
public:
  explicit GameSimulation(std::unique_ptr<Scene> scene);
  ~GameSimulation() = default;

  void Update(float dt);
  void AddEntity();
  void SetEntityIdGenerator(std::function<size_t()> generator);
  void SetEntityFactory(std::function<void(Scene*, std::function<size_t()>&)> factory);
  Scene* GetScene() const { return _scene.get(); }
  std::unique_ptr<Scheduler> _scheduler;
private:
  std::unique_ptr<Scene> _scene;
  std::function<size_t()> _entity_id_generator;
  std::function<void(Scene*, std::function<size_t()>&)> _entity_factory;
};

} // namespace engine

#endif // GAME_SIMULATION_H
