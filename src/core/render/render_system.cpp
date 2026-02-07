#include "core/render/render_system.h"

namespace engine {

RenderSystem::RenderSystem(const RenderConfig& config)
  : _config(config),
    _initialized(false)
{
  InitWindow(_config.width, _config.height, _config.title);
  SetTargetFPS(_config.target_fps);
  _initialized = true;
}

RenderSystem::~RenderSystem()
{
  if (_initialized) {
    CloseWindow();
  }
}

void RenderSystem::BeginFrame()
{
  BeginDrawing();
  ClearBackground(RAYWHITE);
}

void RenderSystem::EndFrame()
{
  EndDrawing();
}

void RenderSystem::RenderScene(const Scene* scene) const
{
  if (scene) {
    scene->Draw();
  }
}

bool RenderSystem::ShouldClose() const
{
  return WindowShouldClose();
}

float RenderSystem::GetDeltaTime() const
{
  return GetFrameTime();
}

} // namespace engine
