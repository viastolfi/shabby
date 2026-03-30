#include "server/logic/example_logic.h"
#include "entities/controllers/entity_controller.h"
#include "networking/server.h"
#include "actors/player_controller_base.h"

class AIController : public Shabby::IEntityController {
public:
  void OnInit(Shabby::Entity* entity) override {
    entity->_pos = {300.f, 200.f};
    entity->SetVelocity(50);
  }
  void OnUpdate(Shabby::Entity* entity, float dt) override {
    _time += dt;
    entity->_pos.x = 300.f + 100.f * cosf(_time);
    entity->_pos.y = 200.f + 100.f * sinf(_time);
  }
private:
  float _time = 0.0f;
};

class ServerPlayerController : public PlayerControllerBase {
};

void ExampleLogic::OnStart(Shabby::Scene* scene, Shabby::Scheduler* scheduler, Shabby::Server* server) 
{
  std::cout << "[ExampleLogic] Server started" << std::endl;
  (void)scene;
  (void)scheduler;
  (void)server;
  /*
  scheduler->Every(5.0f, [scene, server]() {
    std::cout << "[ExampleLogic] Spawning entity..." << std::endl;
    auto* entity = scene->AddEntity(std::make_unique<AIController>(), 1);
    
    Shabby::NetworkPacket packet(Shabby::PacketType::ENTITY_SPAWN);
    packet.Write(static_cast<uint64_t>(entity->_id));
    packet.Write(entity->_pos);
    packet.Write(static_cast<uint32_t>(1));
    server->BroadcastToAll(packet);
    std::cout << "[ExampleLogic] Broadcasted entity " << entity->_id << " to all clients" << std::endl;
  });
  */
}

void ExampleLogic::OnUpdate(Shabby::Scene* scene, float dt)
{
  (void)scene;
  (void)dt;
}

void ExampleLogic::RegisterCustomPackets(Shabby::PacketRegistry* registry)
{
  (void)registry;
}

void ExampleLogic::OnClientConnect(int client_id, Shabby::Scene* scene)
{
  std::cout << "[ExampleLogic] Client " << client_id << " joined the game" << std::endl;
  (void)scene;
}

void ExampleLogic::OnClientDisconnect(
    int client_id, Shabby::Scene* scene)
{
  std::cout << "[ExampleLogic] Client " << client_id << " left the game" << std::endl;
  (void)scene;
}

std::unique_ptr<Shabby::IEntityController> ExampleLogic::CreateControllerFor(
    int texture_id)
{
  (void)texture_id;
  return std::make_unique<ServerPlayerController>();
}
