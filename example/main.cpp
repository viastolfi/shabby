#include "core/engine/engine.h"
#include "game/player.h"
#include "game/main_scene.h"
#include "core/assets/assets_registry.h"
#include "networking/server.h"
#include "example_logic.h"
#include <memory>

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
        TextureId asset_id = static_cast<TextureId>(texture_id);
        int cols = reg.GetTextureCols(asset_id);
        int rows = reg.GetTextureRows(asset_id);
        return std::make_unique<engine::AnimatedSprite>(
            reg, asset_id, cols, rows, 3);
      }
  );

  auto scene = std::make_unique<game::MainScene>();
  size_t player_id;

  player_id = scene->AddEntityWithServerId<game::Player>(
      std::make_unique<engine::AnimatedSprite>
      (registry, TextureId::Monk, 6, 1, 3),
      Vector2{100.0f, 100.0f});

  engine.SetLocalPlayerId(player_id);
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
