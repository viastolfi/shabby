#ifndef EXAMPLE_LOGIC_H
#define EXAMPLE_LOGIC_H

#include "networking/server_logic.h"
#include <iostream>
#include <cmath>

class ExampleLogic : public engine::ServerLogic {
public:
  void OnStart(engine::Scene* scene, engine::Scheduler* scheduler, engine::Server* server) override;
  void OnUpdate(engine::Scene* scene, float dt) override;
  void RegisterCustomPackets(engine::PacketRegistry* registry) override;
  void OnClientConnect(int client_id, engine::Scene* scene) override;
  void OnClientDisconnect(int client_id, engine::Scene* scene) override;
  std::unique_ptr<engine::IEntityController> CreateControllerFor(int texture_id) override;
};

#endif // EXAMPLE_LOGIC_H
