#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <functional>
#include "scene/scene.h"
#include "networking/network_manager.h"

namespace engine {

enum EngineMode {
  STANDALONE,
  CLIENT,
  SERVER,
  HOST
};

struct EngineConfig {
  int width;
  int height;
  const char* title;
  EngineMode mode = STANDALONE;
};

class Engine {
public:
  explicit Engine(const EngineConfig& config);
  ~Engine();
  
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(Engine&&) = default;
  Engine& operator=(Engine&&) = default;
  
  void LoadScene(std::unique_ptr<Scene> s);
  void Run();
  void SetLocalPlayerId(size_t id) { _local_player_id = id; }
  
private:
  EngineConfig _config;
  std::unique_ptr<Scene> _loaded_scene;
  std::unique_ptr<NetworkManager> _network_manager;
  bool _initialized;
  size_t _local_player_id = 0;

  void RunGame();
  void RunServer();
};

} // namespace engine

#endif // ENGINE_H
