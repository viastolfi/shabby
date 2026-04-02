#include <memory>

#include "core/engine/engine.h"
#include "node/sprite/sprite.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"
#include "utils/raylog.h"
#include "node/timer.h"

#include "main_scene.h"
#include "player.h"
#include "ennemy.h"

enum class AssetId {
  BEAF = 0,
  MONKEY_IDLE,
  MONKEY_WALK
};

int main(void)
{
  Raylog::GetInstance(1).Log(1, "Logger Set");

  auto engine = std::make_unique<Shabby::Core::Engine>();

  auto render_system = engine->GetRenderSystem();

  auto assets_registry = 
    std::make_unique<Shabby::Core::AssetRegistry>();

  assets_registry->LoadAll(
    Shabby::Core::AssetDesc{
      static_cast<int>(AssetId::BEAF),
      "assets/Beaf.png" 
    },
    Shabby::Core::AssetDesc{
      static_cast<int>(AssetId::MONKEY_IDLE),
      "assets/actors/monkey/Idle.png" 
    },
    Shabby::Core::AssetDesc{
      static_cast<int>(AssetId::MONKEY_WALK),
      "assets/actors/monkey/Walk.png" 
    }
  );

  auto ms = std::make_shared<MainScene>();

  auto p = std::make_shared<Player>((Vector2){100, 100});

  auto animation_player =
    std::make_shared<Shabby::Node::AnimationPlayer>();

  auto s =
    std::make_shared<Shabby::Node::AnimatedSprite>(
      (Vector2){100, 100}, 
      assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_IDLE)),
      0, 3, 0, 0, 4, 1, 3.0f
    );

  auto walk_down =
    std::make_shared<Shabby::Node::AnimatedSprite>(
      (Vector2){100, 100},
      assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
      0, 0, 0, 3, 4, 4, 3.0f    
    );

  auto walk_up =
    std::make_shared<Shabby::Node::AnimatedSprite>(
      (Vector2){100, 100},
      assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
      1, 1, 0, 3, 4, 4, 3.0f    
    );

  auto walk_left =
    std::make_shared<Shabby::Node::AnimatedSprite>(
      (Vector2){100, 100},
      assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
      2, 2, 0, 3, 4, 4, 3.0f    
    );

  auto walk_right =
    std::make_shared<Shabby::Node::AnimatedSprite>(
      (Vector2){100, 100},
      assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
      3, 3, 0, 3, 4, 4, 3.0f    
    );

  animation_player->Register("idle", s);
  animation_player->Register("walk_up", walk_up);
  animation_player->Register("walk_down", walk_down);
  animation_player->Register("walk_left", walk_left);
  animation_player->Register("walk_right", walk_right);

  animation_player->Play("idle");
  render_system->Register(animation_player.get());

  auto timer =
    std::make_shared<Shabby::Node::Timer>(.1f);

  timer->timeout.connect([assets_registry = assets_registry.get(), ms, render_system]() {
      std::shared_ptr<Shabby::Node::INode> e =
      std::make_shared<Ennemy>((Vector2){200, 200});
  
    auto es =
      std::make_shared<Shabby::Node::Sprite>(
        (Vector2){200, 200},
        assets_registry->GetTexture(static_cast<int>(AssetId::BEAF))
      );

    render_system->Register(es.get());
    e->AddChild(es);
    ms->AddChildDeffered(e);
  });

  p->AddChild(animation_player);
  auto c = p->GetChildOfType<Shabby::Node::Sprite>();

  ms->AddChild(timer);
  ms->AddChild(p);

  timer->Start();
  engine->SetAssetRegistry(std::move(assets_registry));
  engine->LoadTree(ms);
  engine->Run();

  return 0;
}
