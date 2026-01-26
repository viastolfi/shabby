#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <functional>
#include "engine/entities/entity_manager.h"

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
  
  void AddEntity(std::unique_ptr<Entity> entity);
  
  void Run(std::function<void(Engine&)> setup_callback);
  
private:
  EngineConfig _config;
  std::unique_ptr<EntityManager> _entity_manager;
  bool _initialized;
};

} // namespace engine

#endif // ENGINE_H
