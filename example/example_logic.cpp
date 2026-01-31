#include "example_logic.h"

void ExampleLogic::OnStart(engine::GameSimulation& sim) 
{
  sim.SetEntityFactory([](engine::Scene* scene, std::function<size_t()>& id_gen) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist_x(50.0f, 750.0f);
    static std::uniform_real_distribution<float> dist_y(50.0f, 400.0f);
    
    size_t entity_id = id_gen();
    Vector2 position = {dist_x(gen), dist_y(gen)};
    int texture_id = 0;
    
    scene->AddEntity(std::make_unique<engine::Entity>(
        std::make_unique<engine::AnimatedSprite>(texture_id, 4, 1, 3),
        position,
        entity_id));
  });

  sim._scheduler->Every(4.f, [&sim]() {
      sim.AddEntity();
  });
}
