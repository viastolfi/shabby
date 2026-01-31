#include "core/game_simulation/game_simulation.h"
#include "core/sprite/sprite.h"
#include <random>

namespace engine {

GameSimulation::GameSimulation(std::unique_ptr<Scene> scene)
  : _scheduler(std::make_unique<Scheduler>()),
    _scene(std::move(scene)),
    _entity_id_generator(nullptr),
    _entity_factory(nullptr)
{}

void GameSimulation::Update(float dt)
{
  if (_scheduler)
    _scheduler->Update(dt);
}

void GameSimulation::SetEntityIdGenerator(std::function<size_t()> generator)
{
  _entity_id_generator = generator;
}

void GameSimulation::SetEntityFactory(std::function<void(Scene*, std::function<size_t()>&)> factory)
{
  _entity_factory = factory;
}

void GameSimulation::AddEntity()
{
  if (_entity_factory && _scene && _entity_id_generator) {
    _entity_factory(_scene.get(), _entity_id_generator);
  }
}

} // namespace engine
