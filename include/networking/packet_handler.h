#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "networking/protocol/network_packet.h"
#include "scene/scene.h"
#include "core/factories/sprite_factory.h"

namespace Shabby {

class PacketHandler {
public:
  PacketHandler(Scene* scene, size_t local_player_id, const SpriteFactory* sprite_factory);
  ~PacketHandler() = default;
  
  void HandlePacket(NetworkPacket& packet);
  
private:
  Scene* _scene;
  size_t _local_player_id;
  const SpriteFactory* _sprite_factory;
  
  void HandleWorldSnapshot(NetworkPacket& packet);
  void HandleEntityCreateResponse(NetworkPacket& packet);
};

} // namespace Shabby

#endif // PACKET_HANDLER_H
