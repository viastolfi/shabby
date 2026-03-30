#ifndef IPACKET_HANDLER_H
#define IPACKET_HANDLER_H

#include "networking/protocol/network_packet.h"
#include "scene/scene.h"

namespace Shabby {

class IPacketHandler {
public:
  virtual ~IPacketHandler() = default;
 
  virtual NetworkPacket* HandleFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene) = 0;
  
  virtual void HandleFromServer(
      NetworkPacket& packet,
      Scene* scene) = 0;
};

} // namespace Shabby

#endif // IPACKET_HANDLER_H
