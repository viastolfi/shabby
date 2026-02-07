#include "core/engine/engine.h"
#include "core/assets/assets_registry.h"
#include "networking/server.h"
#include "server/logic/example_logic.h"
#include <memory>
#include "actors/player_controller.h"

enum class TextureId
{
  Monkey,
  Monk
};

void RunServerEngine()
{
  engine::ServerConf conf{8080, 2};
  std::unique_ptr<engine::ServerLogic> logic = 
    std::make_unique<ExampleLogic>();
  engine::Engine engine(conf, std::move(logic));
  engine.Run();
}

void RunClientEngine() 
{
  engine::EngineConfig config{
    800,
    450,
    "shabby",
    engine::CLIENT,
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
  auto controller = std::make_unique<PlayerController>(engine.GetClient());
  auto* local_entity = scene->AddEntity(
      std::move(controller), 
      static_cast<int>(TextureId::Monkey));
  local_entity->_is_local = true;
  std::cout << "[Main] Created local entity with temp id=" << local_entity->_id << std::endl;

  engine.LoadScene(std::move(scene));
  engine.Run();
}

int main(int argc, char* argv[]) 
{
  auto has_arg = [argc, argv](const char* lo) -> bool 
  {
    for (int i = 0; i < argc; i++)
     if (strcmp(argv[i], lo) == 0)
      return true; 

    return false;
  };

  if (has_arg("client")) {
    RunClientEngine();
  }
  else if (has_arg("server")) {
    RunServerEngine();
  }
  
  return 0;
}
