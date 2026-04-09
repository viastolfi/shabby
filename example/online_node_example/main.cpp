#include <memory>

#include "utils/raylog.h"
#include "core/engine/engine.h"

int main(int argc, char** argv)
{
  Raylog::GetInstance(1).Log(1, "Logger Set");
  
  // --- Engine setup ---
  auto engine = std::make_unique<Shabby::Core::Engine>(Shabby::Core::EngineMode::SERVER);

  engine->Run();
  return 0;
}
