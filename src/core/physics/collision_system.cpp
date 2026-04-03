#include "core/physics/collision_system.h"

namespace Shabby::Core {

void CollisionSystem::Register(ICollider* c)
{
  _colliders.push_back(c);
}

void CollisionSystem::BroadPhase()
{
  _grid.clear();

  // TODO: make this dependant on the window / camera / dev choice
  int cols = static_cast<int>(800 / 200);
  int rows = static_cast<int>(600 / 200);

  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      _grid[Vector2{static_cast<float>(i), static_cast<float>(j)}] = {};
    }
  }

  for (const auto& e : _colliders) {
    Vector2 v = {
      static_cast<float>(static_cast<int>(e->GetPosition().x / 200)),
      static_cast<float>(static_cast<int>(e->GetPosition().y / 200))
    };

    _grid[v].push_back(e);
  }
}

void CollisionSystem::NarrowPhase()
{
  int cols = static_cast<int>(800 / 200);
  int rows = static_cast<int>(600 / 200);

  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      Vector2 v = {static_cast<float>(i), static_cast<float>(j)};
      if (_grid[v].size() <= 1)
       continue; 

      for (auto& e1 : _grid[v]) {
        for (auto& e2 : _grid[v]) {
          if (e1 == e2)
            continue;

          if (CheckCollisionRecs(
                e1->GetShape(),
                e2->GetShape())) {
            if (!e1->GetIsEnteredState()) 
              e1->OnEnter(e2);
          } else {
            if (e1->GetIsEnteredState()) 
              e1->OnExit(e2);
          }
        } 
      }
    }  
  }
}

} // namespace Shabby::Core
