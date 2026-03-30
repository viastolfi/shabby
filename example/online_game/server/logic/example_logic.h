#ifndef EXAMPLE_LOGIC_H
#define EXAMPLE_LOGIC_H

#include "networking/server_logic.h"
#include <iostream>
#include <cmath>

class ExampleLogic : public Shabby::ServerLogic {
public:
  void OnStart(Shabby::Scene* scene, Shabby::Scheduler* scheduler, Shabby::Server* server) override;
  void OnUpdate(Shabby::Scene* scene, float dt) override;
  void RegisterCustomPackets(Shabby::PacketRegistry* registry) override;
  void OnClientConnect(int client_id, Shabby::Scene* scene) override;
  void OnClientDisconnect(int client_id, Shabby::Scene* scene) override;
  std::unique_ptr<Shabby::IEntityController> CreateControllerFor(int texture_id) override;
};

#endif // EXAMPLE_LOGIC_H
