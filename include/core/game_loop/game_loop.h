#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "scene/scene.h"
#include "core/render/render_system.h"
#include "physics/collision/collision_system.h"
#include "utils/raylog.h"

#include <functional>
#include <chrono>
#include "raylib.h"

namespace Shabby {

struct GameLoopConfig {
  float target_fps = 60.0f;
  bool fixed_timestep = false;
  float fixed_dt = 1.0f / 60.0f;
};

class GameLoop {
public:
  explicit GameLoop(const GameLoopConfig& config = GameLoopConfig{});
  ~GameLoop() = default;
  
  GameLoop(const GameLoop&) = delete;
  GameLoop& operator=(const GameLoop&) = delete;
  
  void Run(
      Scene* scene,
      RenderSystem* render_system = nullptr,
      std::function<bool()> should_continue = nullptr,
      std::function<void()> on_frame_start = nullptr,
      std::function<void(float)> on_tick = nullptr);
  
  void SetCollisionSystem(std::unique_ptr<CollisionSystem> cs);
private:
  GameLoopConfig _config;
  float _accumulator;
  std::unique_ptr<CollisionSystem> _collision_system;
};

} // namespace Shabby

#endif // GAME_LOOP_H
