#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "node/inode.h"
#include "raylib.h"

namespace Shabby::Core {

class RenderSystem {
public:
  RenderSystem();
  ~RenderSystem();
  
  void BeginFrame();
  void EndFrame();
  void RenderTree(const std::shared_ptr<Node::INode> root_node) const;
  
  bool ShouldClose() const;
  float GetDeltaTime() const;
  
private:
  bool _initialized;
};

} // namespace Shabby::Core

#endif // RENDER_SYSTEM_H
