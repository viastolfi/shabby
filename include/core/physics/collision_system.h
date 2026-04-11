#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <algorithm>
#include <vector>
#include <unordered_map>

#include "node/inode.h"
#include "utils/vector2.h"
#include "core/physics/icollider.h"

namespace Shabby::Core {

class CollisionSystem {
public:
  CollisionSystem() = default;
  ~CollisionSystem() = default;

  void Register(ICollider* c);
  void Unregister(ICollider* c);

  void BroadPhase();
  void NarrowPhase();
private:
  std::unordered_map<Vector2, std::vector<ICollider*>> _grid;
  std::vector<ICollider*> _colliders;
};

} // namespace Shabby::Core

#endif // COLLISION_SYSTEM_H
