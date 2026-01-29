#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include "entities/entities.h"
#include "utils/vector2.h"

namespace engine {

class EntityManager {
public:
  EntityManager() = default;
  ~EntityManager() = default;
  
  // delete copy
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;

  // accept move
  EntityManager(EntityManager&&) = default;
  EntityManager& operator=(EntityManager&&) = default;
  
  void AddEntity(std::unique_ptr<Entity> entity);
  
  void UpdateOne(size_t id, Vector2 direction);
  void UpdateAll(float dt);
  void DrawAll() const;
  void UpdatePosition(size_t id, Vector2 pos);

  const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return _entities; }
  
  size_t Count() const { return _entities.size(); }
  
private:
  std::vector<std::unique_ptr<Entity>> _entities;
};

} // namespace engine

#endif // ENTITY_MANAGER_H
