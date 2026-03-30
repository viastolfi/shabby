#ifndef SERVER_LOGIC_H
#define SERVER_LOGIC_H

#include "scene/scene.h"
#include "core/scheduler/scheduler.h"
#include "networking/protocol/packet_registry.h"
#include "replication/snapshot/world_snapshot.h"
#include "networking/handlers/entity_spawn_handler.h"
#include "networking/handlers/input_command_handler.h"
#include "entities/controllers/entity_controller.h"
#include <errno.h>
#include <memory>

namespace Shabby {

class Server;

class ServerLogic {
public:
  virtual ~ServerLogic() = default;
  virtual void OnStart(
      Scene* scene, Scheduler* scheduler, Server* server) 
  { (void)scene; (void)scheduler; (void)server; };
  virtual void OnUpdate(
      Scene* scene, float dt) 
  { (void)scene; (void)dt; };
  virtual void RegisterCustomPackets(
      PacketRegistry* registry) { (void)registry; };
  virtual void OnClientConnect(
      int client_id, Scene* scene) { (void)client_id; (void)scene; };
  virtual void OnClientDisconnect(
      int client_id, Scene* scene) { (void)client_id; (void)scene; };
  virtual std::unique_ptr<IEntityController> CreateControllerFor(
      int texture_id) { (void)texture_id; return nullptr; };
};

} // namespace Shabby 

#endif // SERVER_LOGIC_H
