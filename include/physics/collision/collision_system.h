#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <unordered_map>
#include "scene/scene.h"
#include "entities/entity.h"

namespace engine {

class CollisionSystem {
public:
  CollisionSystem() = default;
  ~CollisionSystem() = default;

  void BroadPhase(const Scene* s);
private:
  std::unordered_map<size_t, std::vector<Entity*>> _grid;
};

} // namespace engine

#endif // COLLISION_SYSTEM_H
