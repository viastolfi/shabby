#include "core/engine/engine.h"

namespace Shabby::Core {

Engine::Engine() 
{
  _game_loop = std::make_unique<GameLoop>();
  _render_system = std::make_shared<RenderSystem>();
  _collision_system = std::make_shared<CollisionSystem>();
}

void Engine::LoadTree(std::shared_ptr<Node::INode> root)
{
  if (root) 
    _root_tree = root;
}

void Engine::SetAssetRegistry(std::unique_ptr<AssetRegistry> as)
{
  _asset_registry = std::move(as);
}

void Engine::Run() 
{
  _game_loop->Run(
    _root_tree, 
    _render_system,
    _collision_system,
    [this]() { 
      return _render_system && !_render_system->ShouldClose(); 
    }
  );
}

std::shared_ptr<RenderSystem> Engine::GetRenderSystem() const
{
  return _render_system;
}

std::shared_ptr<CollisionSystem> Engine::GetCollisionSystem() const
{
  return _collision_system;
}

} // namespace Shabby::Core
