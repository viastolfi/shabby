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
  auto has_arg = [argc, argv](const char* lo) -> bool 
  {
    for (int i = 0; i < argc; i++)
     if (strcmp(argv[i], lo) == 0)
      return true; 

    return false;
  };

  engine::EngineConfig config{
    800,
    450,
    "shabby",
    engine::STANDALONE,
  };  

  if (argc > 1) {
    if (has_arg("server"))
     config.mode = engine::SERVER;
    if (has_arg("client"))
      config.mode = engine::CLIENT;
  }

  engine::Engine engine(config);

  if (config.mode != engine::SERVER) {
    auto& registry = engine.GetAssetRegistry<TextureId>();
    registry.LoadAll(
        engine::AssetDesc<TextureId> { 
          TextureId::Monkey, 
          "assets/actors/monkey/Idle.png",
          4, 1},
        engine::AssetDesc<TextureId> { 
          TextureId::Monk, 
          "assets/actors/monk/Idle.png",
          6, 1}
    );

    engine.SetSpriteFactory<TextureId>(
      [](
        engine::AssetRegistry<TextureId>& reg, int texture_id) 
          -> std::unique_ptr<engine::Sprite> 
      {
        const char* path = 
          reg.GetTexturePath(static_cast<TextureId>(texture_id));
        int cols = 
          reg.GetTextureCols(static_cast<TextureId>(texture_id));
        int rows = 
          reg.GetTextureRows(static_cast<TextureId>(texture_id));
        return std::make_unique<engine::AnimatedSprite>(
            path, cols, rows, 3);
      }
    );

    auto scene = std::make_unique<game::MainScene>();
    size_t player_id;

    if (has_arg("2")) {
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

