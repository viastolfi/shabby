#include "core/physics/collision_system.h"

#include <cmath>

namespace Shabby::Core {

static constexpr float CELL_SIZE = 200.f;

void CollisionSystem::Register(ICollider* c)
{
  _colliders.push_back(c);
}

void CollisionSystem::Unregister(ICollider* c)
{
  _colliders.erase(
    std::remove(_colliders.begin(), _colliders.end(), c),
    _colliders.end());

  // Drop any active pairs involving this collider (no exit event — object is leaving the scene).
  for (auto it = _active_pairs.begin(); it != _active_pairs.end(); ) {
    if (it->first == c || it->second == c)
      it = _active_pairs.erase(it);
    else
      ++it;
  }
}

void CollisionSystem::BroadPhase()
{
  _grid.clear();

  for (const auto& c : _colliders) {
    Rectangle shape = c->GetShape();

    int x0 = static_cast<int>(std::floor(shape.x                  / CELL_SIZE));
    int y0 = static_cast<int>(std::floor(shape.y                  / CELL_SIZE));
    int x1 = static_cast<int>(std::floor((shape.x + shape.width)  / CELL_SIZE));
    int y1 = static_cast<int>(std::floor((shape.y + shape.height) / CELL_SIZE));

    for (int cx = x0; cx <= x1; ++cx)
      for (int cy = y0; cy <= y1; ++cy)
        _grid[Vector2{ (float)cx, (float)cy }].push_back(c);
  }
}

void CollisionSystem::NarrowPhase()
{
  std::set<ColliderPair> new_pairs;

  for (auto& [cell, colliders] : _grid) {
    if (colliders.size() <= 1) continue;

    for (size_t i = 0; i < colliders.size(); ++i) {
      for (size_t j = i + 1; j < colliders.size(); ++j) {
        ICollider* a = colliders[i];
        ICollider* b = colliders[j];

        uint32_t aBit = 1u << (a->GetCollisionLayer() - 1);
        uint32_t bBit = 1u << (b->GetCollisionLayer() - 1);

        bool aDetectsB = (a->GetCollisionMask() & bBit) != 0;
        bool bDetectsA = (b->GetCollisionMask() & aBit) != 0;

        if (!aDetectsB && !bDetectsA) continue;

        if (CheckCollisionRecs(a->GetShape(), b->GetShape()))
          new_pairs.insert(MakePair(a, b));
      }
    }
  }

  for (auto& [a, b] : new_pairs) {
    if (_active_pairs.find({a, b}) == _active_pairs.end()) {
      uint32_t aBit = 1u << (a->GetCollisionLayer() - 1);
      uint32_t bBit = 1u << (b->GetCollisionLayer() - 1);

      if (a->GetCollisionMask() & bBit) a->OnEnter(b);
      if (b->GetCollisionMask() & aBit) b->OnEnter(a);
    }
  }

  for (auto& [a, b] : _active_pairs) {
    if (new_pairs.find({a, b}) == new_pairs.end()) {
      uint32_t aBit = 1u << (a->GetCollisionLayer() - 1);
      uint32_t bBit = 1u << (b->GetCollisionLayer() - 1);

      if (a->GetCollisionMask() & bBit) a->OnExit(b);
      if (b->GetCollisionMask() & aBit) b->OnExit(a);
    }
  }

  _active_pairs = std::move(new_pairs);
}

} // namespace Shabby::Core
