#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <functional>
#include "engine/scene/scene.h"

namespace engine {

struct EngineConfig {
  int width;
  int height;
  const char* title;
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
  
private:
  EngineConfig _config;
  std::unique_ptr<Scene> _loaded_scene;
  bool _initialized;
};

} // namespace engine

#endif // ENGINE_H
