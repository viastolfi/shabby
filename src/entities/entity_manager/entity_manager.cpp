#include "entities/entity_manager/entity_manager.h"

namespace engine {

EntityManager::EntityManager()
{
  _hitbox_factory = std::make_unique<HitboxFactory>();
}

EntityManager::~EntityManager()
{
  for (auto* entity : _entities) {
    delete entity;
  }
  _entities.clear();
}

Entity* EntityManager::AddEntity(
    std::unique_ptr<IEntityController> controller,
    int texture_id) 
{
  if (!_entity_factory) {
    std::cerr << "[EntityManager] Error: EntityFactory not initialized!" << std::endl;
    return nullptr;
  }
  
  Entity* e = _entity_factory->CreateEntity(
      std::move(controller), texture_id);
  if (e) {
    e->_id = GenerateEntityId();
    e->_texture_id = texture_id;
    _entities.push_back(e);
    e->Init();
    if (e->GetHitboxCreateFunction())
      e->SetHitbox(_hitbox_factory->CreateHitbox(*e));
  }
  return e;
}

Entity* EntityManager::AddEntity(
    std::unique_ptr<IEntityController> controller,
    int texture_id,
    float x, float y, float width, float height)
{
  Entity* e = _entity_factory->CreateEntity(
      std::move(controller), texture_id);
  
  // TODO: add memory safety

  e->_id = GenerateEntityId();
  e->_texture_id = texture_id;
  _entities.push_back(e);
  e->Init();
  e->SetHitbox(_hitbox_factory->CreateBaseRectangleHitbox(
        x, y, width, height));

  return e;
}

Entity* EntityManager::AddEntity(
    uint64_t id,
    int texture_id,
    Vector2 position)
{
  Entity* e = _entity_factory->CreateEntity(nullptr, texture_id);
  e->_id = id;
  e->_pos = position;
  e->_texture_id = texture_id;

  _entities.push_back(e);
  return e;
}


void EntityManager::AddEntity(
    std::unique_ptr<IEntityController> controller,
    std::unique_ptr<AnimationPlayer> ap)
{
  Entity* e = _entity_factory->CreateEntity(
      std::move(controller), std::move(ap));
  e->_id = GenerateEntityId();
  e->Init();

  _entities.push_back(e);
}

void EntityManager::RemoveEntity(uint64_t id)
{
  for (auto it = _entities.begin(); it != _entities.end(); ++it) {
    if ((*it)->_id == id) {
      delete *it;
      _entities.erase(it);
      return;
    }
  }
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

void EntityManager::SetEntityFactory(
    std::unique_ptr<EntityFactory> factory)
{
  _entity_factory = std::move(factory);
}

size_t EntityManager::GenerateEntityId()
{
  return _next_entity_id++;
}

} // namespace engine
