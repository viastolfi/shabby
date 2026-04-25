#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "node/inode.h"
#include "raylib.h"

namespace Shabby::Core {
class RenderSystem;
}

namespace Shabby::Node {

class Camera2DNode : public INode {
public:
  // offset: screen-space point where the target appears (default: centre of an 800x600 window)
  Camera2DNode(std::weak_ptr<Core::RenderSystem> render_system,
               Vector2 offset = {400.0f, 300.0f},
               float zoom = 1.0f,
               float rotation = 0.0f);
  ~Camera2DNode();

  void Update(float dt) override;

  Camera2D GetCamera2D() const;
  void SetZoom(float zoom);
  void SetRotation(float rotation);
  void SetOffset(Vector2 offset);

private:
  Camera2D _camera;
  std::weak_ptr<Core::RenderSystem> _render_system;
};

} // namespace Shabby::Node

#endif // CAMERA_H
