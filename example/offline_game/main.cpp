#include <memory>
#include "actors/player_controller.h"
#include "actors/ennemy_controller.h"
#include "core/engine/engine.h"
#include "scene/scene.h"
#include "raylib.h"

enum class TextureId
{
  Monkey,
  Monk
};

int main() 
{
  Raylog::GetInstance(1).Log(1, "Logger set");
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

  scene->ScheduleTaskAfter(1.f, [](engine::Scene* s) {
      s->AddEntity(
          std::make_unique<EnnemyController>(),
          static_cast<int>(TextureId::Monkey)); 
  });

  engine.LoadScene(std::move(scene));
  engine.Run();
}
