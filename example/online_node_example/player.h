#ifndef ONLINE_PLAYER_H
#define ONLINE_PLAYER_H

#include "node/collision_shape/rigid_body.h"
#include "node/sprite/animation_player.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/icollider.h"
#include "utils/raylog.h"

class Player : public Shabby::Node::RigidBody {
public:
  static constexpr float SPEED = 200.0f;

  Player(Vector2 pos, bool is_authority)
    :Shabby::Node::RigidBody(pos), _is_authority(is_authority)
  {}

  ~Player() = default;

  void SetInput(float dx, float dy) { _input_dx = dx; _input_dy = dy; }
  float GetInputDx() const { return _input_dx; }
  float GetInputDy() const { return _input_dy; }

  void _physics_process(float dt) override
  {
    (void)dt;

    if (_is_authority) {
      _dir = {_input_dx * SPEED, _input_dy * SPEED};

      if (_pos.x <= 0 && _dir.x < 0) _dir.x = 0;
      if (_pos.x >= 784 && _dir.x > 0) _dir.x = 0;
      if (_pos.y <= 0 && _dir.y < 0) _dir.y = 0;
      if (_pos.y >= 584 && _dir.y > 0) _dir.y = 0;
    } else {
      _dir = {0, 0};
    }

    auto anims = GetChildOfType<Shabby::Node::AnimationPlayer>();
    if (!anims.empty()) {
      auto ap = anims.front();
      if (_input_dx == 0 && _input_dy == 0)
        ap->Play("idle");
      else if (_input_dy > 0)
        ap->Play("walk_down");
      else if (_input_dy < 0)
        ap->Play("walk_up");
      else if (_input_dx < 0)
        ap->Play("walk_left");
      else if (_input_dx > 0)
        ap->Play("walk_right");
    }
  }

  void _on_hitbox_entered(
      Shabby::Node::RectangleHitbox* hit,
      Shabby::Core::ICollider* from) override
  {
    (void)hit;
    (void)from;
    Raylog::GetInstance().Log(1, "HITBOX ENTERED");
  }

  void _on_hitbox_exited(
      Shabby::Node::RectangleHitbox* hit,
      Shabby::Core::ICollider* from) override
  {
    (void)hit;
    (void)from;
    Raylog::GetInstance().Log(1, "HITBOX EXITED");
  }

private:
  bool _is_authority;
  float _input_dx = 0, _input_dy = 0;
};

#endif // ONLINE_PLAYER_H
