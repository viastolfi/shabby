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

  auto engine = std::make_unique<Shabby::Core::Engine>();

  auto render_system = engine->GetRenderSystem();

  auto assets_registry = 
    std::make_unique<Shabby::Core::AssetRegistry>();

  assets_registry->LoadAll(
    Shabby::Core::AssetDesc{
      static_cast<int>(AssetId::BEAF),
      "assets/Beaf.png" 
    }
  );

  auto ms = std::make_shared<MainScene>();

  auto p = std::make_shared<Player>((Vector2){100, 100});

  auto s =
    std::make_shared<Shabby::Node::Sprite>(
      (Vector2){100, 100}, 
      assets_registry->GetTexture(static_cast<int>(AssetId::BEAF))
    );
  render_system->Register(s.get());

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

  p->AddChild(s);
  auto c = p->GetChildOfType<Shabby::Node::Sprite>();

  ms->AddChild(timer);
  ms->AddChild(p);

  timer->Start();
  engine->SetAssetRegistry(std::move(assets_registry));
  engine->LoadTree(ms);
  engine->Run();

  return 0;
}
