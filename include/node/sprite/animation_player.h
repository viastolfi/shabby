#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H

#include <memory>
#include <string>
#include <map>

#include "node/inode.h"
#include "node/sprite/animated_sprite.h"
#include "core/drawable/idrawable.h"
#include "core/drawable/ibounded.h"

namespace Shabby::Node {

class AnimationPlayer final : public INode, public Core::IDrawable, public Core::IBounded {
public:
  AnimationPlayer() = default;
  ~AnimationPlayer() = default;

  void Register(std::string name, std::weak_ptr<AnimatedSprite> as);
  void Play(std::string name);
  void Update(float dt) override;
  void Draw() override;
  Rectangle GetBounds() const override;
private:
  std::map<std::string, std::weak_ptr<AnimatedSprite>> _animations;
  std::shared_ptr<AnimatedSprite> _current_animation;
};

} // namespace Shabby::Node

#endif // ANIMATION_PLAYER_H
