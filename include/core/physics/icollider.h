#ifndef ICOLLIDER_H
#define ICOLLIDER_H

#include <cassert>
#include <cstdint>
#include "raylib.h"

namespace Shabby::Core {

static constexpr int kMaxCollisionLayers = 10;

class ICollider {
public:
  virtual ~ICollider() = default;

  virtual Rectangle      GetShape()    const = 0;
  virtual const Vector2  GetPosition() const = 0;

  virtual void OnEnter(ICollider* other) = 0;
  virtual void OnExit(ICollider* other)  = 0;

  void SetCollisionLayer(int layer)
  {
    assert(layer >= 1 && layer <= kMaxCollisionLayers);
    _collision_layer = layer;
  }

  int GetCollisionLayer() const { return _collision_layer; }

  void SetCollisionMask(uint32_t mask) { _collision_mask = mask; }
  uint32_t GetCollisionMask() const    { return _collision_mask; }

  void AddMaskLayer(int layer)
  {
    assert(layer >= 1 && layer <= kMaxCollisionLayers);
    _collision_mask |= (1u << (layer - 1));
  }

  void RemoveMaskLayer(int layer)
  {
    assert(layer >= 1 && layer <= kMaxCollisionLayers);
    _collision_mask &= ~(1u << (layer - 1));
  }

private:
  int      _collision_layer = 1;
  uint32_t _collision_mask  = 0;
};

} // namespace Shabby::Core

#endif // ICOLLIDER_H
