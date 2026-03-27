#include "physics/collision/collision_system.h"

namespace engine {

CollisionSystem::CollisionSystem(double w, double h)
  :_window_width(w), _window_height(h)
{}

void CollisionSystem::BroadPhase(const Scene* s)
{
  _grid.clear();

  int cols = static_cast<int>(_window_width / MAX_WIDTH);
  int rows = static_cast<int>(_window_height / MAX_HEIGHT);

  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      _grid[Vector2{static_cast<float>(i), static_cast<float>(j)}] = {};
    }
  }

  for (auto e : s->GetEntities()) {
    if (e->GetHitbox() == nullptr) 
      continue; 

    Vector2 v = {
      static_cast<float>(static_cast<int>(e->_pos.x / MAX_WIDTH)),
      static_cast<float>(static_cast<int>(e->_pos.y / MAX_HEIGHT))
    };
    _grid[v].push_back(e);
  }
}

void CollisionSystem::NarrowPhase()
{
  int cols = static_cast<int>(_window_width / MAX_WIDTH);
  int rows = static_cast<int>(_window_height / MAX_HEIGHT);

  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      Vector2 v = {static_cast<float>(i), static_cast<float>(j)};

      if (_grid[v].size() <= 1) 
        continue;

      for (auto e1 : _grid[v]) {
        for (auto e2 : _grid[v]) {
          if (e1->_id == e2->_id)
            continue; 

          if (CheckCollisionRecs(
                e1->GetHitbox()->GetShape(),
                e2->GetHitbox()->GetShape())) {
            if (!e1->GetHitbox()->_is_entered) {
              e1->OnHitboxEntered(e1->GetHitbox(), e2->GetHitbox());
            }
          } else {
            if (e1->GetHitbox()->_is_entered)
             e1->OnHitboxExited(e1->GetHitbox(), e2->GetHitbox());
          }
        } 
      }  
    }
  }
}

} // namespace engine
