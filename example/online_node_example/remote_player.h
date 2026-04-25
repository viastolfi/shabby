#ifndef REMOTE_PLAYER_H
#define REMOTE_PLAYER_H

#include "node/inode.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"

#include "raylib.h"

#include <memory>

// Client-side visual representation of a player.
// Does NOT register with RenderSystem or call AddChild — the owner
// (ClientScene) performs both operations after construction.
class RemotePlayer : public Shabby::Node::INode {
public:
  RemotePlayer(Vector2 pos, Texture2D idle, Texture2D walk)
    : INode(pos)
  {
    auto anim_idle  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, idle, 0, 3, 0, 0, 4, 1, 3.0f);
    auto walk_down  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 0, 0, 0, 3, 4, 4, 3.0f);
    auto walk_up    = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 1, 1, 0, 3, 4, 4, 3.0f);
    auto walk_left  = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 2, 2, 0, 3, 4, 4, 3.0f);
    auto walk_right = std::make_shared<Shabby::Node::AnimatedSprite>(pos, walk, 3, 3, 0, 3, 4, 4, 3.0f);

    _anim = std::make_shared<Shabby::Node::AnimationPlayer>();
    _anim->Register("idle",       anim_idle);
    _anim->Register("walk_down",  walk_down);
    _anim->Register("walk_up",    walk_up);
    _anim->Register("walk_left",  walk_left);
    _anim->Register("walk_right", walk_right);
    _anim->Play("idle");
  }

  ~RemotePlayer() = default;

  std::shared_ptr<Shabby::Node::AnimationPlayer> GetAnimPlayer() const { return _anim; }

  void SetAnimation(float dx, float dy)
  {
    if (dx == 0 && dy == 0)     _anim->Play("idle");
    else if (dy > 0)            _anim->Play("walk_down");
    else if (dy < 0)            _anim->Play("walk_up");
    else if (dx < 0)            _anim->Play("walk_left");
    else                        _anim->Play("walk_right");
  }

private:
  std::shared_ptr<Shabby::Node::AnimationPlayer> _anim;
};

#endif // REMOTE_PLAYER_H
