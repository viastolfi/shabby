#ifndef EXAMPLE_LOGIC_H
#define EXAMPLE_LOGIC_H

#include "networking/server_logic.h"
#include "core/game_simulation/game_simulation.h"
#include "game/player.h"
#include <random>


class ExampleLogic : public engine::ServerLogic {
public:
  void OnStart(engine::GameSimulation& sim) override;
};

#endif // EXAMPLE_LOGIC_H
