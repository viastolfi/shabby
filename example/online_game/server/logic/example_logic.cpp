#include "server/logic/example_logic.h"
#include "entities/controllers/entity_controller.h"
#include "networking/server.h"

class AIController : public engine::IEntityController {
public:
  void OnInit(engine::Entity* entity) override {
    entity->_pos = {300.f, 200.f};
    entity->SetVelocity(50);
  }
  void OnUpdate(engine::Entity* entity, float dt) override {
    _time += dt;
    entity->_pos.x = 300.f + 100.f * cosf(_time);
    entity->_pos.y = 200.f + 100.f * sinf(_time);
  }
private:
  float _time = 0.0f;
};

void ExampleLogic::OnStart(engine::Scene* scene, engine::Scheduler* scheduler, engine::Server* server) 
{
  std::cout << "[ExampleLogic] Server started" << std::endl;
  scheduler->Every(5.0f, [scene, server]() {
    std::cout << "[ExampleLogic] Spawning entity..." << std::endl;
    auto* entity = scene->AddEntity(std::make_unique<AIController>(), 1);
    
    engine::NetworkPacket packet(engine::PacketType::ENTITY_SPAWN);
    packet.Write(static_cast<uint64_t>(entity->_id));
    packet.Write(entity->_pos);
    packet.Write(static_cast<uint32_t>(1));
    server->BroadcastToAll(packet);
    std::cout << "[ExampleLogic] Broadcasted entity " << entity->_id << " to all clients" << std::endl;
  });
}

void ExampleLogic::OnUpdate(engine::Scene* scene, float dt)
{
  (void)scene;
  (void)dt;
}

void ExampleLogic::RegisterCustomPackets(engine::PacketRegistry* registry)
{
  (void)registry;
}

void ExampleLogic::OnClientConnect(int client_id, engine::Scene* scene)
{
  std::cout << "[ExampleLogic] Client " << client_id << " joined the game" << std::endl;
  (void)scene;
}

void ExampleLogic::OnClientDisconnect(
    int client_id, engine::Scene* scene)
{
  std::cout << "[ExampleLogic] Client " << client_id << " left the game" << std::endl;
  (void)scene;
}
