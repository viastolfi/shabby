#ifndef REMOTE_PLAYER_H
#define REMOTE_PLAYER_H

#include "node/inode.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"

#include "raylib.h"

#include <memory>
#include <string>
#include <vector>
#include <cmath>

// Client-side visual representation of a player.
// Does NOT register with RenderSystem or call AddChild — the owner
// (ClientScene) performs both operations after construction.
class RemotePlayer : public Shabby::Node::INode {
public:
  RemotePlayer(Vector2 pos, Texture2D idle, Texture2D walk)
    : INode(pos)
  {
    // NinjaBlue Idle.png: 64×16, 4 cols × 1 row, one frame per direction
    // col 0=down, 1=left, 2=right, 3=up
    auto idle_down  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, idle, 0, 0, 0, 0, 4, 1, 1.0f);
    auto idle_left  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, idle, 1, 1, 0, 0, 4, 1, 1.0f);
    auto idle_right = std::make_shared<Shabby::Node::AnimatedSprite>(pos, idle, 2, 2, 0, 0, 4, 1, 1.0f);
    auto idle_up    = std::make_shared<Shabby::Node::AnimatedSprite>(pos, idle, 3, 3, 0, 0, 4, 1, 1.0f);

    // NinjaBlue Walk.png: 64×64, 4 cols × 4 rows
    // row 0=down, 1=left, 2=right, 3=up; 4 frames each
    auto walk_down  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 0, 3, 0, 0, 4, 4, 8.0f);
    auto walk_left  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 0, 3, 1, 1, 4, 4, 8.0f);
    auto walk_right = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 0, 3, 2, 2, 4, 4, 8.0f);
    auto walk_up    = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 0, 3, 3, 3, 4, 4, 8.0f);

    _sprites = { idle_down, idle_left, idle_right, idle_up,
                 walk_down, walk_left, walk_right, walk_up };

    _anim = std::make_shared<Shabby::Node::AnimationPlayer>();
    _anim->Register("idle_down",  idle_down);
    _anim->Register("idle_left",  idle_left);
    _anim->Register("idle_right", idle_right);
    _anim->Register("idle_up",    idle_up);
    _anim->Register("walk_down",  walk_down);
    _anim->Register("walk_left",  walk_left);
    _anim->Register("walk_right", walk_right);
    _anim->Register("walk_up",    walk_up);
    _anim->Play("idle_down");
  }

  std::shared_ptr<Shabby::Node::AnimationPlayer> GetAnimPlayer() const { return _anim; }

  void SetAnimation(float dx, float dy)
  {
    std::string next;
    if (std::fabsf(dx) < 0.5f && std::fabsf(dy) < 0.5f) {
      if (_last_dir == "down")  next = "idle_down";
      else if (_last_dir == "left")  next = "idle_left";
      else if (_last_dir == "right") next = "idle_right";
      else                           next = "idle_up";
    } else if (std::abs(dy) >= std::abs(dx)) {
      next = (dy > 0) ? "walk_down" : "walk_up";
      _last_dir = (dy > 0) ? "down" : "up";
    } else {
      next = (dx < 0) ? "walk_left" : "walk_right";
      _last_dir = (dx < 0) ? "left" : "right";
    }
    if (next != _current_anim) {
      _anim->Play(next);
      _current_anim = next;
    }
  }

private:
  std::vector<std::shared_ptr<Shabby::Node::AnimatedSprite>> _sprites;
  std::shared_ptr<Shabby::Node::AnimationPlayer>             _anim;
  std::string _last_dir    = "down";
  std::string _current_anim = "idle_down";
};

#endif // REMOTE_PLAYER_H
