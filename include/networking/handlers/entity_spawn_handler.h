#ifndef ENTITY_SPAWN_HANDLER_H
#define ENTITY_SPAWN_HANDLER_H

#include "networking/protocol/ipacket_handler.h"
#include "entities/controllers/entity_controller.h"
#include "raylib.h"
#include <sstream>

namespace engine {

class EntitySpawnHandler : public IPacketHandler {
public:
  EntitySpawnHandler() = default;
  ~EntitySpawnHandler() = default;
  
  NetworkPacket* HandleFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene) override;
  
  void HandleFromServer(
      NetworkPacket& packet,
      Scene* scene) override;
};

} // namespace engine

#endif // ENTITY_SPAWN_HANDLER_H
