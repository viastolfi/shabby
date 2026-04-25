#include "node/hitbox/rectangle_hitbox.h"

namespace Shabby::Node {

RectangleHitbox::RectangleHitbox(Rectangle s)
  :INode(), _shape(s)
{}

void RectangleHitbox::Draw()
{
  if (Raylog::GetInstance().GetLogLevel() >= 1)
    DrawRectangleLinesEx(_shape, 1, BLUE);
}

void RectangleHitbox::Update(float dt)
{
  (void)dt;
  if (auto parent = _parent.lock())
    _pos = parent->GetPos();

  _shape.x = _pos.x;
  _shape.y = _pos.y;
}

Rectangle RectangleHitbox::GetShape() const
{
  return _shape;
}

Rectangle RectangleHitbox::GetBounds() const
{
  return GetShape();
}

void RectangleHitbox::OnEnter(Core::ICollider* other)
{
  entered.emit(this, other);
}

void RectangleHitbox::OnExit(Core::ICollider* other)
{
  exited.emit(this, other);
}

const Vector2 RectangleHitbox::GetPosition() const
{
  return _pos;
}

} // namespace Shabby::Node
