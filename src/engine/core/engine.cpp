#include "engine.h"
#include "raylib.h"

namespace engine {

Engine::Engine(const EngineConfig& config) 
  : _config(config), 
    _entity_manager(std::make_unique<EntityManager>()), 
    _initialized(false) 
{}

Engine::~Engine() 
{
  _entity_manager.reset();
  if (_initialized) 
    CloseWindow();
}

void Engine::AddEntity(std::unique_ptr<Entity> entity) 
{
  if (_entity_manager) 
    _entity_manager->AddEntity(std::move(entity));
}

void Engine::Run(std::function<void(Engine&)> setup_callback) 
{
  InitWindow(_config.width, _config.height, _config.title);
  SetTargetFPS(60);
  _initialized = true;
  
  if (setup_callback) 
    setup_callback(*this);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (_entity_manager) 
      _entity_manager->UpdateAll(dt);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (_entity_manager) 
      _entity_manager->DrawAll();

    EndDrawing();
  }
}

} // namespace engine
