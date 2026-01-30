#include "core/engine/engine.h"
#include "game/player.h"
#include "game/main_scene.h"
#include "core/assets/assets_registry.h"
#include <memory>

enum class TextureId
{
  Monkey,
  Monk
};

int main(int argc, char* argv[]) 
{
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
  //
  // TODO: make this all easier to use for game maker
  if (config.mode != engine::SERVER) {
    engine::AssetRegistry<TextureId> registry;
    registry.LoadAll(
        engine::AssetDesc<TextureId> { TextureId::Monkey, "assets/actors/monkey/Idle.png"},
        engine::AssetDesc<TextureId> { TextureId::Monk, "assets/actors/monk/Idle.png"}
        );

    auto scene = std::make_unique<game::MainScene>();
    size_t player_id;

    if (argc > 2 && strcmp(argv[2], "2") == 0) {
      player_id = scene->AddEntityWithServerId<game::Player>(
          std::make_unique<engine::AnimatedSprite>
          (registry, TextureId::Monkey, 4, 1, 3),
          Vector2{100.0f, 100.0f});
    } else {
      player_id = scene->AddEntityWithServerId<game::Player>(
          std::make_unique<engine::AnimatedSprite>
          (registry, TextureId::Monk, 6, 1, 3),
          Vector2{100.0f, 100.0f});

    }

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

