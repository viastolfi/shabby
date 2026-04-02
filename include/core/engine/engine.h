#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "core/assets/assets_registry.h"
#include "node/inode.h"
#include "core/game_loop/game_loop.h"

namespace Shabby::Core {

class Engine {
public:
  Engine();
  ~Engine() = default;

  void LoadTree(std::shared_ptr<Node::INode> root);
  
  void SetAssetRegistry(std::unique_ptr<AssetRegistry> as);

  void Run();

  std::shared_ptr<RenderSystem> GetRenderSystem() const;
private:
  std::unique_ptr<AssetRegistry> _asset_registry;
  std::unique_ptr<GameLoop> _game_loop;
  std::shared_ptr<RenderSystem> _render_system;

  std::shared_ptr<Node::INode> _root_tree;
};

} // namespace Shabby::Core

#endif // ENGINE_H
