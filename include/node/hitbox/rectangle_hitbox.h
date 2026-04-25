#ifndef RECTANGLE_HITBOX_H
#define RECTANGLE_HITBOX_H

#include "node/inode.h"
#include "core/drawable/idrawable.h"
#include "core/drawable/ibounded.h"
#include "core/signal/signal.h"
#include "core/physics/icollider.h"
#include "utils/raylog.h"

namespace Shabby::Node {

class RectangleHitbox : public INode, public Core::IDrawable, public Core::IBounded, public Core::ICollider {
public:
  RectangleHitbox(Rectangle s);
  ~RectangleHitbox() = default;

  Core::Signal<RectangleHitbox*, Core::ICollider*> entered;
  Core::Signal<RectangleHitbox*, Core::ICollider*> exited;

  Rectangle GetShape() const override;
  Rectangle GetBounds() const override;
  void OnEnter(Core::ICollider* other) override;
  void OnExit(Core::ICollider* other) override;
  const Vector2 GetPosition() const override;

  void Draw() override final;
  void Update(float dt) override final;
private:
  Rectangle _shape;
};

} // namespace Shabby::Node

#endif // RECTANGLE_HITBOX_H
