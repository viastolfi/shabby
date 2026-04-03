#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "node/inode.h"
#include "utils/vector2.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/icollider.h"

#include "raymath.h"

namespace Shabby::Node {

class RigidBody : public INode {
public:
  RigidBody(Vector2 pos);
  ~RigidBody() = default;

  void Update(float dt) override final;

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
};
  
} // namespace Shabby::Node

#endif // RIGID_BODY_H
