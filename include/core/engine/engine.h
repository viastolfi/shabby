#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <functional>
#include <unordered_map>
#include <typeindex>
#include "scene/scene.h"
#include "networking/network_manager.h"
#include "core/assets/assets_registry.h"
#include "core/factories/sprite_factory.h"
#include "networking/packet_handler.h"
#include "raylib.h"

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
  void SetLocalPlayerId(size_t id) { _local_player_id = id; }
  
  template<typename T>
  AssetRegistry<T>& GetAssetRegistry() {
    if (!_assets_registry) {
      _assets_registry = std::make_shared<AssetRegistry<T>>();
      _assets_registry_type = std::type_index(typeid(T));
    }
    if (_assets_registry_type != std::type_index(typeid(T))) {
      throw std::runtime_error(
          "AssetRegistry already initialized with a different type");
    }
    return *std::static_pointer_cast<AssetRegistry<T>>(_assets_registry);
  }
  
  template<typename T>
  void SetSpriteFactory(
      std::function<std::unique_ptr<Sprite>
        (AssetRegistry<T>&, int)> factory) {
    auto wrapped_factory = [this, factory](int texture_id) 
      -> std::unique_ptr<Sprite> {
        auto& registry = this->GetAssetRegistry<T>();
        return factory(registry, texture_id);
      };
    _sprite_factory.SetFactory(wrapped_factory);
  }
  
  SpriteFactory& GetSpriteFactory() { return _sprite_factory; }
  
private:
  EngineConfig _config;

  std::shared_ptr<void> _assets_registry;
  std::type_index _assets_registry_type{typeid(void)};
  SpriteFactory _sprite_factory;

  std::unique_ptr<Scene> _loaded_scene;
  std::unique_ptr<NetworkManager> _network_manager;
  bool _initialized;
  size_t _local_player_id = 0;

  void RunGame();
  void RunServer();
};

} // namespace engine

#endif // ENGINE_H
