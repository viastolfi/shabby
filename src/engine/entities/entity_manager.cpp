#include "entity_manager.h"

namespace engine {

void EntityManager::AddEntity(std::unique_ptr<Entity> entity) 
{
  if (entity) 
    _entities.push_back(std::move(entity));
}

void EntityManager::UpdateAll(float dt) 
{
  for (auto& entity : _entities) 
    if (entity) 
      entity->Update(dt);
}

void EntityManager::DrawAll() const 
{
  for (const auto& entity : _entities) 
    if (entity) 
      entity->Draw();
}

} // namespace engine
