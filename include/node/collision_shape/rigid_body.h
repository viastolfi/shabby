#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <cstdint>

#include "node/inode.h"
#include "utils/vector2.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/icollider.h"
#include "core/physics/collision_system.h"

#include "raymath.h"

namespace Shabby::Node {

class RigidBody : public INode {
public:
  RigidBody(Vector2 pos);
  ~RigidBody() = default;

  void Update(float dt) override final;

  void SetCollisionSystem(Core::CollisionSystem* cs) { _collision_system = cs; }
  void SetSolidMask(uint32_t mask)                   { _solid_mask = mask; }
  void AddSolidMaskLayer(int layer)                  { _solid_mask |= (1u << (layer - 1)); }
  void RemoveSolidMaskLayer(int layer)               { _solid_mask &= ~(1u << (layer - 1)); }

  virtual void _physics_process(float dt) = 0;

  virtual void _on_hitbox_entered(RectangleHitbox* hit, Core::ICollider* from)
  {
    (void)hit;
    (void)from;
  };

  virtual void _on_hitbox_exited(RectangleHitbox* hit, Core::ICollider* from)
  {
    (void)hit;
    (void)from;
  }

protected:
  Vector2 _dir;

private:
  Core::CollisionSystem* _collision_system = nullptr;
  uint32_t               _solid_mask       = 0;
  RectangleHitbox*       _body_hitbox      = nullptr;
};

} // namespace Shabby::Node

#endif // RIGID_BODY_H
