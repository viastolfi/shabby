#include "core/engine/engine.h"

namespace Shabby::Core {

Engine::Engine(EngineMode mode) 
  :_mode(mode)
{
  if (_mode != EngineMode::SERVER)
    _render_system = std::make_shared<RenderSystem>();

  _game_loop = std::make_unique<GameLoop>();
  _collision_system = std::make_shared<CollisionSystem>();
}

void Engine::LoadTree(std::shared_ptr<Node::INode> root)
{
  if (!root) 
    throw std::invalid_argument("root cannot be null");

  if (_mode == EngineMode::SERVER) {
    auto r = std::dynamic_pointer_cast<Node::Server>(root); 
    if (r == nullptr)
      throw std::runtime_error("Root must be a Server node in SERVER mode");
  }

  _root_tree = root;
}

void Engine::SetAssetRegistry(std::unique_ptr<AssetRegistry> as)
{
  _asset_registry = std::move(as);
}

void Engine::Run() 
{
  if (_root_tree == nullptr)
    throw std::runtime_error("Root tree must be setted before running engine");

  if (_mode == EngineMode::STADALONE) {
    _game_loop->Run(
        _root_tree, 
        _render_system,
        _collision_system,
        [this]() { 
          return _render_system && !_render_system->ShouldClose(); 
        }
    );
  }
  else if (_mode == EngineMode::SERVER) {
    _game_loop->Run(
      _root_tree,
      nullptr,
      _collision_system,
      [this]() {
        return true; 
      }
    );
  }
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
