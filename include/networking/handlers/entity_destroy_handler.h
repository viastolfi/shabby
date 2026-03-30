#ifndef ENTITY_DESTROY_HANDLER_H
#define ENTITY_DESTROY_HANDLER_H

#include "networking/protocol/network_packet.h"
#include "networking/protocol/ipacket_handler.h"
#include "scene/scene.h"

namespace Shabby {

class EntityDestroyHandler : public IPacketHandler {
public:
  NetworkPacket* HandleFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene) override;
  
  void HandleFromServer(
      NetworkPacket& packet,
      Scene* scene) override;
};

} // namespace Shabby

#endif // ENTITY_DESTROY_HANDLER_H
