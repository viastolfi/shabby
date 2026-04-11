#ifndef STANDALONE_LOOP_CONTROLLER_H
#define STANDALONE_LOOP_CONTROLLER_H

#include <memory>

#include "core/game_loop/iloop_controller.h"
#include "core/render/render_system.h"

namespace Shabby::Core {

class StandaloneLoopController : public ILoopController {
public:
  StandaloneLoopController(std::shared_ptr<RenderSystem> r);
  ~StandaloneLoopController() = default;

  bool ShouldContinue() override;
private:
  std::shared_ptr<RenderSystem> _render_system;
};

} // namespace Shabby::Core

#endif // STANDALONE_LOOP_CONTROLLER_H
