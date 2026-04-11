#ifndef PLAYER_H
#define PLAYER_H

#include "node/collision_shape/rigid_body.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/icollider.h"

class Player : public Shabby::Node::RigidBody {
public:
  const float SPEED = 200.0f;

  Player(Vector2 pos)
    :Shabby::Node::RigidBody(pos)
  {}

  void _physics_process(float dt) override
  {
    (void)dt;
    _dir = {0, 0};

    auto animation_player = GetChildOfType<Shabby::Node::AnimationPlayer>().front();

    if (IsKeyDown(KEY_A)) _dir.x -= 1 * SPEED;
    if (IsKeyDown(KEY_D)) _dir.x += 1 * SPEED;
    if (IsKeyDown(KEY_W)) _dir.y -= 1 * SPEED;
    if (IsKeyDown(KEY_S)) _dir.y += 1 * SPEED;

    if (_dir.x == 0 && _dir.y == 0)
      animation_player->Play("idle");
    else if (_dir.y > 0)
      animation_player->Play("walk_down");
    else if (_dir.y < 0)
      animation_player->Play("walk_up");
    else if (_dir.x < 0)
      animation_player->Play("walk_left");
    else if (_dir.x > 0)
      animation_player->Play("walk_right");    
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

  ~Player() = default;
};

#endif // PLAYER_H
