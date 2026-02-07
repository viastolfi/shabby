#include "scene/scene.h"

namespace engine {

Scene::Scene(AssetRegistry* assets_registry)
  : _task_scheduler(std::make_unique<Scheduler>()),
    _entity_manager(std::make_unique<EntityManager>())
{
  auto sprite_factory = std::make_unique<SpriteFactory>(assets_registry);
  _entity_manager->SetEntityFactory(
      std::make_unique<EntityFactory>(std::move(sprite_factory)));
}

Scene::Scene()
  : _task_scheduler(std::make_unique<Scheduler>()),
    _entity_manager(std::make_unique<EntityManager>())
{
  auto sprite_factory = std::make_unique<SpriteFactory>(nullptr);
  _entity_manager->SetEntityFactory(
      std::make_unique<EntityFactory>(std::move(sprite_factory)));
}

Scene::~Scene()
{
  _entity_manager.reset();
}

void Scene::Update(float dt)
{
  if (_entity_manager)
    _entity_manager->UpdateAll(dt);
  if (_task_scheduler)
    _task_scheduler->Update(dt);
}

void Scene::Draw() const
{
  if (_entity_manager)
    _entity_manager->DrawAll();
}

Entity* Scene::AddEntity(
    std::unique_ptr<IEntityController> controller,
    int texture_id)
{
  return _entity_manager->AddEntity(std::move(controller), texture_id);
}

void Scene::AddEntity(
      uint64_t id,
      int texture_id,
      Vector2 position)
{
  _entity_manager->AddEntity(id, texture_id, position);
}

void Scene::RemoveEntity(uint64_t id)
{
  _entity_manager->RemoveEntity(id);
}

void Scene::ScheduleTaskAfter(float seconds, std::function<void(Scene*)> task)
{
  if (_task_scheduler) {
    _task_scheduler->After(seconds, [this, task]() {
      task(this);
    });
  }
}

void Scene::ScheduleTaskEvery(float seconds, std::function<void(Scene*)> task)
{
  if (_task_scheduler) {
    _task_scheduler->Every(seconds, [this, task]() {
      task(this);
    });
  }
}

const std::vector<Entity*>& Scene::GetEntities() const
{
  return _entity_manager->GetEntities();
}

WorldSnapshot Scene::CreateWorldSnapshot() const
{
  WorldSnapshot snapshot;
  for (const auto* entity : _entity_manager->GetEntities()) {
    snapshot.AddEntity(entity->_id, entity->_pos, entity->GetSpriteTextureId());
  }
  return snapshot;
}

void Scene::ApplyWorldSnapshot(WorldSnapshot& snapshot)
{
  for (const auto& entity_data : snapshot.GetEntities()) {
    for (auto* entity : _entity_manager->GetEntities()) {
      if (entity->_id == entity_data.id && !entity->_is_local) {
        entity->_pos = entity_data.position;
        break;
      }
    }
  }
}
 
} // namespace engine
