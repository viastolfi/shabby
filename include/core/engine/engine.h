#ifndef ENGINE_H
#define ENGINE_H

#include "scene/scene.h"
#include "networking/server.h"
#include "networking/client.h"
#include "networking/protocol/packet_registry.h"
#include "core/assets/assets_registry.h"
#include "core/factories/sprite_factory.h"
#include "core/render/render_system.h"
#include "core/game_loop/game_loop.h"
#include "raylib.h"
#include <memory>
#include "networking/handlers/entity_spawn_handler.h"
#include "networking/handlers/entity_destroy_handler.h"
#include "networking/handlers/snapshot_handler.h"

namespace engine {

enum EngineMode {
  STANDALONE,
  CLIENT,
  SERVER,
  HOST
};

struct EngineConfig {
  int width = 0;
  int height = 0;
  const char* title = "";
  EngineMode mode = STANDALONE;
};

class Engine {
public:
  explicit Engine(const EngineConfig& config);
  explicit Engine(
      const ServerConf& server_config,
      std::unique_ptr<ServerLogic> logic);
  ~Engine();
  
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(Engine&&) = default;
  Engine& operator=(Engine&&) = default;
  
  void LoadScene(std::unique_ptr<Scene> s);
  void Run();
  
  AssetRegistry& GetAssetRegistry() {
    if (!_assets_registry) {
      _assets_registry = std::make_shared<AssetRegistry>();
    }
    return *_assets_registry;
  }
  
  Client* GetClient() { return _client.get(); }

 private:
  EngineConfig _config;

  std::shared_ptr<AssetRegistry> _assets_registry;

  std::unique_ptr<Scene> _loaded_scene;
  std::unique_ptr<Server> _server;
  std::unique_ptr<Client> _client;
  std::unique_ptr<PacketRegistry> _packet_registry;
  std::unique_ptr<RenderSystem> _render_system;
  std::unique_ptr<GameLoop> _game_loop;
  bool _initialized;
};

} // namespace engine

#endif // ENGINE_H
