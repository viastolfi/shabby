#include "core/engine/engine.h"
#include "game/player.h"
#include "game/main_scene.h"
#include <memory>

int main(int argc, char* argv[]) {
  engine::EngineConfig config{
    800,
    450,
    "shabby",
    engine::STANDALONE,
  };  

  if (argc > 1) {
    if (strcmp(argv[1], "server") == 0)
     config.mode = engine::SERVER;
    if (strcmp(argv[1], "client") == 0)
      config.mode = engine::CLIENT;
  }

  engine::Engine engine(config);

  // TODO: make this all easier to use for game maker
  if (config.mode != engine::SERVER) {
    auto scene = std::make_unique<game::MainScene>();

    size_t player_id = scene->AddEntityWithServerId<game::Player>(
        std::make_unique<engine::AnimatedSprite>
        ("assets/actors/monkey/Idle.png", 4, 1, 3), 
        Vector2{100.0f, 100.0f});

    engine.SetLocalPlayerId(player_id);
    engine.LoadScene(std::move(scene));
  }
  else {
    auto scene = std::make_unique<game::MainScene>();

    engine.LoadScene(std::move(scene));
  }
  engine.Run();
  
  return 0;
}

