#include "node/collision_shape/rigid_body.h"

#include "raylib.h"

namespace Shabby::Node {

RigidBody::RigidBody(Vector2 pos)
  :INode(pos)
{}

void RigidBody::Update(float dt)
{
  _physics_process(dt);

  if (_collision_system && _solid_mask != 0) {
    if (!_body_hitbox) {
      auto children = GetChildOfType<RectangleHitbox>();
      if (!children.empty())
        _body_hitbox = children.front().get();
    }

    if (_body_hitbox) {
      Rectangle shape = _body_hitbox->GetShape();
      Rectangle body  = { _pos.x, _pos.y, shape.width, shape.height };

      for (auto* c : _collision_system->GetColliders()) {
        if (c == _body_hitbox) continue;

        uint32_t layerBit = 1u << (c->GetCollisionLayer() - 1);
        if (!(_solid_mask & layerBit)) continue;

        Rectangle other = c->GetShape();

        if (fabsf(_dir.x) > 0.f) {
          Rectangle test = { body.x + _dir.x * dt, body.y, body.width, body.height };
          if (CheckCollisionRecs(test, other))
            _dir.x = 0.f;
        }
        if (fabsf(_dir.y) > 0.f) {
          Rectangle test = { body.x, body.y + _dir.y * dt, body.width, body.height };
          if (CheckCollisionRecs(test, other))
            _dir.y = 0.f;
        }
      }
    }
  }

  _pos = Vector2Add(_pos, Vector2Scale(_dir, dt));

  INode::Update(dt);
}

} // namespace Shabby::Node
