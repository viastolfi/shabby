#include <memory>
#include <string>
#include <cstring>

#include "utils/raylog.h"
#include "core/engine/engine.h"
#include "core/assets/assets_registry.h"

#include "game_server.h"
#include "game_client.h"

enum class AssetId {
  BEAF = 0,
  MONKEY_IDLE,
  MONKEY_WALK
};

int main(int argc, char** argv)
{
  if (argc < 2) {
    Raylog::GetInstance(1).Log(1, "Usage: %s server|client [ip]", argv[0]);
    return 1;
  }

  Raylog::GetInstance(1).Log(1, "Logger Set");

  bool is_server = (std::strcmp(argv[1], "server") == 0);
  std::string ip = "127.0.0.1";
  if (!is_server && argc >= 3)
    ip = argv[2];

  if (is_server) {
    auto engine = std::make_unique<Shabby::Core::Engine>(Shabby::Core::EngineMode::SERVER);
    auto collision = engine->GetCollisionSystem();

    auto scene = std::make_shared<GameServer>(collision.get());
    scene->CreateServer(7777);

    Raylog::GetInstance().Log(1, "Server started on port 7777");

    engine->LoadTree(scene);
    engine->Run();
  } else {
    auto engine = std::make_unique<Shabby::Core::Engine>(Shabby::Core::EngineMode::CLIENT);
    auto render = engine->GetRenderSystem();

    auto assets = std::make_unique<Shabby::Core::AssetRegistry>();
    assets->LoadAll(
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::BEAF), "assets/Beaf.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::MONKEY_IDLE), "assets/actors/monkey/Idle.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::MONKEY_WALK), "assets/actors/monkey/Walk.png"}
    );

    auto scene = std::make_shared<GameClient>(
        render.get(),
        assets->GetTexture(static_cast<int>(AssetId::BEAF)),
        assets->GetTexture(static_cast<int>(AssetId::MONKEY_IDLE)),
        assets->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)));

    scene->ConnectToServer(ip, 7777);
    Raylog::GetInstance().Log(1, "Connecting to %s:7777", ip.c_str());

    engine->SetAssetRegistry(std::move(assets));
    engine->LoadTree(scene);
    engine->Run();
  }

  return 0;
}
