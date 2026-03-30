#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <unordered_map>
#include <math.h>
#include "scene/scene.h"
#include "entities/entity.h"
#include "utils/vector2.h"

namespace Shabby {

class CollisionSystem {
public:
  CollisionSystem(double w, double h);
  ~CollisionSystem() = default;

  void BroadPhase(const Scene* s);
  void NarrowPhase();

  double MAX_WIDTH = 200.0;
  double MAX_HEIGHT = 200.0;
private:
  std::unordered_map<Vector2, std::vector<Entity*>> _grid;
  double _window_width;
  double _window_height;
};

} // namespace Shabby

#endif // COLLISION_SYSTEM_H
