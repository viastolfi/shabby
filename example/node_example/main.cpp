#include <memory>

#include "core/engine/engine.h"
#include "node/sprite.h"
#include "utils/raylog.h"
#include "node/timer.h"

#include "main_scene.h"
#include "player.h"
#include "ennemy.h"

enum class AssetId {
  BEAF = 0
};

int main(void)
{
  Raylog::GetInstance(1).Log(1, "Logger Set");

  std::unique_ptr<Shabby::Core::Engine> engine =
    std::make_unique<Shabby::Core::Engine>();

  std::unique_ptr<Shabby::Core::AssetRegistry> assets_registry =
    std::make_unique<Shabby::Core::AssetRegistry>();

  assets_registry->LoadAll(
    Shabby::Core::AssetDesc{
      static_cast<int>(AssetId::BEAF),
      "assets/Beaf.png" 
    }
  );

  std::shared_ptr<Shabby::Node::INode> ms = 
    std::make_shared<MainScene>();

  std::shared_ptr<Shabby::Node::INode> p = 
    std::make_shared<Player>((Vector2){100, 100});

  std::shared_ptr<Shabby::Node::INode> s =
    std::make_shared<Shabby::Node::Sprite>(
      (Vector2){100, 100}, 
      assets_registry->GetTexture(static_cast<int>(AssetId::BEAF))
    );

  auto timer =
    std::make_shared<Shabby::Node::Timer>(.1f);

  timer->timeout.connect([assets_registry = assets_registry.get(), ms]() {
      std::shared_ptr<Shabby::Node::INode> e =
      std::make_shared<Ennemy>((Vector2){200, 200});
  
    std::shared_ptr<Shabby::Node::INode> es =
      std::make_shared<Shabby::Node::Sprite>(
        (Vector2){200, 200},
        assets_registry->GetTexture(static_cast<int>(AssetId::BEAF))
      );

    e->AddChild(es);
    ms->AddChildDeffered(e);
  });

  p->AddChild(s);

  ms->AddChild(timer);
  ms->AddChild(p);

  timer->Start();
  engine->SetAssetRegistry(std::move(assets_registry));
  engine->LoadTree(ms);
  engine->Run();

  return 0;
}
