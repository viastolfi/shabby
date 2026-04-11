#include "core/game_loop/standalone_loop_controller.h"

namespace Shabby::Core {

StandaloneLoopController::StandaloneLoopController(
    std::shared_ptr<RenderSystem> r)
  :_render_system(r)
{}

bool StandaloneLoopController::ShouldContinue()
{
  return _render_system && !_render_system->ShouldClose();
}

} // namespace Shabby::Core
