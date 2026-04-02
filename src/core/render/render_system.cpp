#include "core/render/render_system.h"

namespace Shabby::Core {

RenderSystem::RenderSystem()
  : _initialized(false)
{
  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  // SetConfigFlags(FLAG_WINDOW_TOPMOST);

  // TODO: make this the choice of the dev
  InitWindow(800, 600, "test");
  SetTargetFPS(60);
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

void RenderSystem::DrawAll()
{
  for (auto& d : _drawables)
    d->Draw();
}

void RenderSystem::Register(IDrawable* d)
{
  _drawables.push_back(d);
}

bool RenderSystem::ShouldClose() const
{
  return WindowShouldClose();
}

float RenderSystem::GetDeltaTime() const
{
  return GetFrameTime();
}

} // namespace Shabby::Core
