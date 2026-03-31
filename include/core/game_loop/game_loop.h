#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <functional>
#include <chrono>

#include "core/render/render_system.h"
#include "utils/raylog.h"
#include "node/inode.h"
#include "raylib.h"

namespace Shabby::Core {

class GameLoop {
public:
  GameLoop() = default;
  ~GameLoop() = default;
  
  void Run(
      std::shared_ptr<Node::INode> root_node,
      std::shared_ptr<RenderSystem> render_system = nullptr,
      std::function<bool()> should_continue = nullptr,
      std::function<void()> on_frame_start = nullptr,
      std::function<void(float)> on_tick = nullptr);
  
  // void SetCollisionSystem(std::unique_ptr<CollisionSystem> cs);
private:
  float _accumulator = 0.0f;
  // std::unique_ptr<CollisionSystem> _collision_system;
};

} // namespace Shabby::Core

#endif // GAME_LOOP_H
