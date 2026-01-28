#include "core/engine/engine.h"
#include "game/player.h"
#include "game/main_scene.h"
#include "game/ennemy.h"
#include <memory>

int main() {
  engine::EngineConfig config{
    800,
    450,
    "shabby"
  };

  engine::Engine engine(config);

  auto scene = std::make_unique<game::MainScene>();

  scene->AddEntity<game::Player>(
      std::make_unique<engine::AnimatedSprite>
        ("assets/actors/monkey/Idle.png", 4, 1, 3), 
      Vector2{100.0f, 100.0f});

  scene->AddEntity<game::Ennemy>(
      std::make_unique<engine::AnimatedSprite>
        ("assets/actors/monk/Idle.png", 6, 1, 5), 
      Vector2{200.0f, 100.0f});
  
  engine.LoadScene(std::move(scene));
  engine.Run();
  
  return 0;
}

