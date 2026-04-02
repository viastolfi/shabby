#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "node/inode.h"
#include "core/drawable/idrawable.h"
#include "raylib.h"

namespace Shabby::Core {

class RenderSystem {
public:
  RenderSystem();
  ~RenderSystem();
  
  void BeginFrame();
  void EndFrame();
  void Register(IDrawable* d);
  void DrawAll();
  
  bool ShouldClose() const;
  float GetDeltaTime() const;
  
private:
  bool _initialized;
  std::vector<IDrawable*> _drawables;
};

} // namespace Shabby::Core

#endif // RENDER_SYSTEM_H
