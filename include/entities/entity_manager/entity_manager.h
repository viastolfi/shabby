#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entities/entity.h"
#include "utils/vector2.h"
#include "core/factories/entity_factory.h"
#include "core/factories/sprite_factory.h"
#include "core/factories/hitbox_factory.h"
#include <vector>
#include <memory>
#include <iostream>

namespace Shabby {

class EntityManager {
public:
  EntityManager(); 
  ~EntityManager();
  
  // delete copy
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;

  // accept move
  EntityManager(EntityManager&&) = default;
  EntityManager& operator=(EntityManager&&) = default;
  
  Entity* AddEntity(
      std::unique_ptr<IEntityController> controller,
      int texture_id);
  
  Entity* AddEntity(
      std::unique_ptr<IEntityController> controller,
      int texture_id,
      float x, float y, float width, float height);

  Entity* AddEntity(
      uint64_t id,
      int texture_id,
      Vector2 position);

  void AddEntity(
      std::unique_ptr<IEntityController> controller,
      std::unique_ptr<AnimationPlayer> ap);

  void RemoveEntity(uint64_t id);

  void UpdateAll(float dt);
  void DrawAll() const;
  const std::vector<Entity*>& GetEntities() const 
  { 
    return _entities; 
  }
  
  size_t Count() const { return _entities.size(); }
  
  size_t GenerateEntityId();
  
  void SetEntityFactory(std::unique_ptr<EntityFactory> factory);
private:
  std::vector<Entity*> _entities;
  std::unique_ptr<EntityFactory> _entity_factory;
  std::unique_ptr<HitboxFactory> _hitbox_factory;
  size_t _next_entity_id = 1;
};

} // namespace Shabby

#endif // ENTITY_MANAGER_H
