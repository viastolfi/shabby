#include "engine.h"
#include "raylib.h"

namespace engine {

Engine::Engine(const EngineConfig& config) 
  : _config(config), 
    _loaded_scene(nullptr),
    _initialized(false) 
{
  InitWindow(_config.width, _config.height, _config.title);
  SetTargetFPS(60);
  _initialized = true;
}

Engine::~Engine() 
{
  if (_initialized) 
    CloseWindow();
}

void Engine::LoadScene(std::unique_ptr<Scene> s)
{
  _loaded_scene = std::move(s);
}

void Engine::Run() 
{
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (_loaded_scene) 
      _loaded_scene->UpdateScene(dt); 

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (_loaded_scene) 
      _loaded_scene->DrawScene();

    EndDrawing();
  }
}

} // namespace engine
