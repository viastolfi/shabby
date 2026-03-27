#include "core/game_loop/game_loop.h"
#include <thread>

namespace engine {

GameLoop::GameLoop(const GameLoopConfig& config)
  : _config(config),
    _accumulator(0.0f)
{}

void GameLoop::Run(
    Scene* scene,
    RenderSystem* render_system,
    std::function<bool()> should_continue,
    std::function<void()> on_frame_start,
    std::function<void(float)> on_tick)
{
  if (!scene) return;
  
  auto default_continue = []() { return true; };
  auto continue_check = should_continue ? should_continue : default_continue;
  auto last_time = std::chrono::high_resolution_clock::now();
  
  while (continue_check()) {
    if (on_frame_start) {
      on_frame_start();
    }
    
    float frame_dt;
    if (render_system) {
      frame_dt = render_system->GetDeltaTime();
    } else {
      auto now = std::chrono::high_resolution_clock::now();
      frame_dt = std::chrono::duration<float>(now - last_time).count();
      last_time = now;
    }
    
    if (_config.fixed_timestep) {
      _accumulator += frame_dt;
      
      while (_accumulator >= _config.fixed_dt) {
        if (on_tick) {
          on_tick(_config.fixed_dt);
        }
        scene->Update(_config.fixed_dt);
        _accumulator -= _config.fixed_dt;
      }
    } else {
      if (on_tick) {
        on_tick(frame_dt);
      }
      scene->Update(frame_dt);
    }
    
    if (_collision_system) {
      _collision_system->BroadPhase(scene);
      _collision_system->NarrowPhase();
    }
    
    if (render_system) {
      render_system->BeginFrame();
      render_system->RenderScene(scene);
      render_system->EndFrame();
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

void GameLoop::SetCollisionSystem(std::unique_ptr<CollisionSystem> cs)
{
  _collision_system = std::move(cs);
}

} // namespace engine
