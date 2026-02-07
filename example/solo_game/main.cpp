#include <memory>
#include "actors/player_controller.h"
#include "core/engine/engine.h"
#include "scene/scene.h"

enum class TextureId
{
  Monkey,
  Monk
};

int main() 
{
  engine::EngineConfig config{
    800,
    450,
    "shabby",
    engine::STANDALONE,
  };  

  engine::Engine engine(config);
  auto& registry = engine.GetAssetRegistry();
  registry.LoadAll(
    engine::AssetDesc { 
      static_cast<int>(TextureId::Monkey), 
      "assets/actors/monkey/Idle.png",
      4, 1},
    engine::AssetDesc { 
      static_cast<int>(TextureId::Monk), 
      "assets/actors/monk/Idle.png",
      6, 1}
  );
  
  auto scene = std::make_unique<engine::Scene>(&registry);
  
  scene->AddEntity(
      std::make_unique<PlayerController>(), 
      static_cast<int>(TextureId::Monkey));

  scene->ScheduleTaskEvery(4.f, [](engine::Scene* s) {
      s->AddEntity(
          std::make_unique<PlayerController>(),
          static_cast<int>(TextureId::Monk)); 
  });

  engine.LoadScene(std::move(scene));
  engine.Run();
}
