#ifndef SERVER_LOGIC_H
#define SERVER_LOGIC_H

#include "core/game_simulation/game_simulation.h"

namespace engine {

class ServerLogic {
public:
  virtual ~ServerLogic() = default;

  virtual void OnStart([[maybe_unused]] GameSimulation& sim) {};
  virtual void OnUpdate([[maybe_unused]] GameSimulation& sim) {};
};

} // namespace engine

#endif // SERVER_LOGIC_H
