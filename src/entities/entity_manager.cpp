#include "entities/entity_manager.h"

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

void EntityManager::UpdateOne(size_t id, Vector2 direction)
{
  for (auto& e : _entities) {
    if (e->GetId() == id) {
      e->SetPos(e->GetPos() + direction);
      break;
    }
  }
}

void EntityManager::UpdatePosition(size_t id, Vector2 pos)
{
  for (auto& e : _entities) {
    if (e->GetId() == id) {
      e->SetPos(pos);
      break;
    }
  }
}

} // namespace engine
