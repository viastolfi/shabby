#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "scene/scene.h"
#include "core/render/render_system.h"
#include <functional>
#include "raylib.h"

namespace engine {

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
      std::function<void()> on_frame_start = nullptr);
  
private:
  GameLoopConfig _config;
  float _accumulator;
};

} // namespace engine

#endif // GAME_LOOP_H
