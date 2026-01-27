#include "engine/core/engine.h"
#include "game/player.h"
#include "game/main_scene.h"
#include <memory>

int main() {
  engine::EngineConfig config{
    800,
    450,
    "shabby"
  };

  engine::Engine engine(config);

  std::unique_ptr<engine::Scene> scene = std::make_unique<game::MainScene>();
  scene->AddEntity(std::make_unique<game::Player>("assets/actors/monkey/Idle.png"));
  
  engine.LoadScene(std::move(scene));
  engine.Run();
  
  return 0;
}

