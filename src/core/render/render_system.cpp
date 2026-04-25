#include "core/render/render_system.h"
#include "node/camera/camera.h"

#include <algorithm>

namespace Shabby::Core {

RenderSystem::RenderSystem()
  : _initialized(false)
{
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
  ClearBackground(BLACK);
}

void RenderSystem::EndFrame()
{
  EndDrawing();
}

void RenderSystem::Register(IDrawable* d)
{
  _drawables.push_back({ d, dynamic_cast<IBounded*>(d) });
}

void RenderSystem::Unregister(IDrawable* d)
{
  _drawables.erase(
    std::remove_if(_drawables.begin(), _drawables.end(),
      [d](const DrawableEntry& e) { return e.drawable == d; }),
    _drawables.end());
}

void RenderSystem::RegisterHUD(IDrawable* d)
{
  _hud_drawables.push_back(d);
}

void RenderSystem::UnregisterHUD(IDrawable* d)
{
  _hud_drawables.erase(
    std::remove(_hud_drawables.begin(), _hud_drawables.end(), d),
    _hud_drawables.end());
}

Rectangle RenderSystem::GetViewport() const
{
  if (!_active_camera) {
    return { 0.f, 0.f, (float)GetScreenWidth(), (float)GetScreenHeight() };
  }

  Camera2D cam = _active_camera->GetCamera2D();

  // Convert the 4 screen corners to world space — handles rotation and zoom correctly.
  Vector2 tl = GetScreenToWorld2D({ 0.f,                     0.f                      }, cam);
  Vector2 tr = GetScreenToWorld2D({ (float)GetScreenWidth(),  0.f                      }, cam);
  Vector2 bl = GetScreenToWorld2D({ 0.f,                     (float)GetScreenHeight()  }, cam);
  Vector2 br = GetScreenToWorld2D({ (float)GetScreenWidth(),  (float)GetScreenHeight() }, cam);

  float minX = std::min({ tl.x, tr.x, bl.x, br.x });
  float minY = std::min({ tl.y, tr.y, bl.y, br.y });
  float maxX = std::max({ tl.x, tr.x, bl.x, br.x });
  float maxY = std::max({ tl.y, tr.y, bl.y, br.y });

  return { minX, minY, maxX - minX, maxY - minY };
}

void RenderSystem::DrawAll()
{
  std::stable_sort(_drawables.begin(), _drawables.end(),
    [](const DrawableEntry& a, const DrawableEntry& b) {
      return a.drawable->GetRenderLayer() < b.drawable->GetRenderLayer();
    });

  std::stable_sort(_hud_drawables.begin(), _hud_drawables.end(),
    [](const IDrawable* a, const IDrawable* b) {
      return a->GetRenderLayer() < b->GetRenderLayer();
    });

  Rectangle viewport = GetViewport();

  auto draw_world = [&]() {
    for (auto& entry : _drawables) {
      if (entry.bounded && !CheckCollisionRecs(entry.bounded->GetBounds(), viewport))
        continue;
      entry.drawable->Draw();
    }
  };

  if (_active_camera) {
    BeginMode2D(_active_camera->GetCamera2D());
    draw_world();
    EndMode2D();
  } else {
    draw_world();
  }

  for (auto* d : _hud_drawables)
    d->Draw();
}

void RenderSystem::SetActiveCamera(Node::Camera2DNode* cam)
{
  _active_camera = cam;
}

void RenderSystem::ClearActiveCamera(Node::Camera2DNode* cam)
{
  if (_active_camera == cam)
    _active_camera = nullptr;
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
