#include "engine/core/engine.h"
#include "game/player.h"
#include <memory>

int main() {
  engine::EngineConfig config{
    800,
    450,
    "shabby"
  };

  engine::Engine engine(config);
  
  engine.Run([](engine::Engine& eng) {
    eng.AddEntity(std::make_unique<game::Player>("assets/Beaf.png"));
  });
  
  return 0;
}

