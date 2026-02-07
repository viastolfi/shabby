#ifndef NETWORKED_ENTITY_CONTROLLER_H
#define NETWORKED_ENTITY_CONTROLLER_H

#include "entities/controllers/entity_controller.h"
#include "networking/protocol/network_packet.h"
#include "networking/client.h"
#include "entities/entity.h"

namespace engine {

class NetworkedEntityController : public IEntityController {
public:
  explicit NetworkedEntityController(Client* client) : _client(client) {}
  
  void OnInit(Entity* entity) override;
  void OnUpdate(Entity* entity, float dt) override;
  
private:
  Client* _client;
};

} // namespace engine

#endif // NETWORKED_ENTITY_CONTROLLER_H

