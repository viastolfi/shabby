#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <algorithm>
#include <vector>

#include "node/inode.h"
#include "core/drawable/idrawable.h"
#include "core/drawable/ibounded.h"
#include "raylib.h"

namespace Shabby::Node {
class Camera2DNode;
}

namespace Shabby::Core {

class RenderSystem {
public:
  RenderSystem();
  ~RenderSystem();

  // World-space drawables — culled against camera viewport when a camera is active.
  void Register(IDrawable* d);
  void Unregister(IDrawable* d);

  // Screen-space (HUD) drawables — never culled, drawn after world pass.
  void RegisterHUD(IDrawable* d);
  void UnregisterHUD(IDrawable* d);

  void BeginFrame();
  void EndFrame();
  void DrawAll();

  void SetActiveCamera(Node::Camera2DNode* cam);
  void ClearActiveCamera(Node::Camera2DNode* cam);

  bool ShouldClose() const;
  float GetDeltaTime() const;

private:
  struct DrawableEntry {
    IDrawable*  drawable;
    IBounded*   bounded; // nullptr when drawable does not implement IBounded
  };

  Rectangle GetViewport() const;

  bool _initialized;
  std::vector<DrawableEntry> _drawables;
  std::vector<IDrawable*>    _hud_drawables;
  Node::Camera2DNode* _active_camera = nullptr;
};

} // namespace Shabby::Core

#endif // RENDER_SYSTEM_H
