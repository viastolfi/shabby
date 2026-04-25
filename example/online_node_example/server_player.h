#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "node/collision_shape/rigid_body.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/icollider.h"
#include "utils/raylog.h"

class ServerPlayer : public Shabby::Node::RigidBody {
public:
  static constexpr float SPEED = 200.0f;

  ServerPlayer(Vector2 pos)
    : Shabby::Node::RigidBody(pos)
  {}

  ~ServerPlayer() = default;

  void SetInput(float dx, float dy) { _input_dx = dx; _input_dy = dy; }
  float GetInputDx() const { return _input_dx; }
  float GetInputDy() const { return _input_dy; }

  void _physics_process(float dt) override
  {
    (void)dt;
    _dir = {_input_dx * SPEED, _input_dy * SPEED};

    if (_pos.x <= 0   && _dir.x < 0) _dir.x = 0;
    if (_pos.x >= 784 && _dir.x > 0) _dir.x = 0;
    if (_pos.y <= 0   && _dir.y < 0) _dir.y = 0;
    if (_pos.y >= 584 && _dir.y > 0) _dir.y = 0;
  }

  void _on_hitbox_entered(
      Shabby::Node::RectangleHitbox* hit,
      Shabby::Core::ICollider* from) override
  {
    (void)hit;
    (void)from;
    Raylog::GetInstance().Log(1, "Player hitbox entered");
  }

  void _on_hitbox_exited(
      Shabby::Node::RectangleHitbox* hit,
      Shabby::Core::ICollider* from) override
  {
    (void)hit;
    (void)from;
    Raylog::GetInstance().Log(1, "Player hitbox exited");
  }

private:
  float _input_dx = 0;
  float _input_dy = 0;
};

#endif // SERVER_PLAYER_H
