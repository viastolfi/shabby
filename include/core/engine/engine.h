#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "core/game_loop/server_loop_controller.h"
#include "core/game_loop/standalone_loop_controller.h"
#include "core/game_loop/iloop_controller.h"
#include "core/assets/assets_registry.h"
#include "node/inode.h"
#include "node/networking/network_node.h"
#include "core/physics/collision_system.h"
#include "core/game_loop/game_loop.h"

namespace Shabby::Core {

enum class EngineMode {
  SERVER,
  CLIENT,
  STADALONE
};

class Engine {
public:
  Engine(EngineMode mode = EngineMode::STADALONE);
  ~Engine() = default;

  void LoadTree(std::shared_ptr<Node::INode> root);
  
  void SetAssetRegistry(std::unique_ptr<AssetRegistry> as);

  void Run();

  std::shared_ptr<RenderSystem> GetRenderSystem() const;
  std::shared_ptr<CollisionSystem> GetCollisionSystem() const;
private:
  std::unique_ptr<AssetRegistry> _asset_registry;
  std::unique_ptr<GameLoop> _game_loop;
  std::shared_ptr<ILoopController> _loop_controller;
  std::shared_ptr<RenderSystem> _render_system;
  std::shared_ptr<CollisionSystem> _collision_system;

  std::shared_ptr<Node::INode> _root_tree;

  EngineMode _mode = EngineMode::STADALONE;
};

} // namespace Shabby::Core

#endif // ENGINE_H
