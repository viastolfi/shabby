#ifndef SERVER_LOOP_CONTROLLER_H
#define SERVER_LOOP_CONTROLLER_H

#include "core/game_loop/iloop_controller.h"

namespace Shabby::Core {

class ServerLoopController : public ILoopController {
public:
  ServerLoopController() = default;
  ~ServerLoopController() = default;

  bool ShouldContinue() override;
};

} // namespace Shabby::Core

#endif // SERVER_LOOP_CONTROLLER_H
