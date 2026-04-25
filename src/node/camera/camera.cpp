#include "node/camera/camera.h"
#include "core/render/render_system.h"

#include <algorithm>

namespace Shabby::Node {

Camera2DNode::Camera2DNode(std::weak_ptr<Core::RenderSystem> render_system,
                           Vector2 offset,
                           float zoom,
                           float rotation)
  : _render_system(render_system)
{
  _camera.offset   = offset;
  _camera.target   = _pos;
  _camera.zoom     = std::max(0.01f, zoom);
  _camera.rotation = rotation;

  if (auto rs = _render_system.lock()) {
    rs->SetActiveCamera(this);
  }
}

Camera2DNode::~Camera2DNode()
{
  if (auto rs = _render_system.lock()) {
    rs->ClearActiveCamera(this);
  }
}

void Camera2DNode::Update(float dt)
{
  if (auto parent = _parent.lock()) {
    _camera.target = parent->GetPos();
  } else {
    _camera.target = _pos;
  }

  INode::Update(dt);
}

Camera2D Camera2DNode::GetCamera2D() const
{
  return _camera;
}

void Camera2DNode::SetZoom(float zoom)
{
  _camera.zoom = std::max(0.01f, zoom);
}

void Camera2DNode::SetRotation(float rotation)
{
  _camera.rotation = rotation;
}

void Camera2DNode::SetOffset(Vector2 offset)
{
  _camera.offset = offset;
}

} // namespace Shabby::Node
