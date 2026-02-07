#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "scene/scene.h"
#include "raylib.h"

namespace engine {

struct RenderConfig {
  int width = 800;
  int height = 450;
  const char* title = "Game";
  int target_fps = 60;
};

class RenderSystem {
public:
  explicit RenderSystem(const RenderConfig& config);
  ~RenderSystem();
  
  RenderSystem(const RenderSystem&) = delete;
  RenderSystem& operator=(const RenderSystem&) = delete;
  
  void BeginFrame();
  void EndFrame();
  void RenderScene(const Scene* scene) const;
  
  bool ShouldClose() const;
  float GetDeltaTime() const;
  
private:
  RenderConfig _config;
  bool _initialized;
};

} // namespace engine

#endif // RENDER_SYSTEM_H
