#include "networking/packet_handler.h"

namespace Shabby {

PacketHandler::PacketHandler(
    Scene* scene, 
    size_t local_player_id, 
    const SpriteFactory* sprite_factory)
  : _scene(scene),
    _local_player_id(local_player_id),
    _sprite_factory(sprite_factory)
{}

void PacketHandler::HandlePacket(NetworkPacket& packet)
{
  switch (packet.GetType()) {
    case PacketType::SNAPSHOT:
      HandleWorldSnapshot(packet);
      break;
    case PacketType::ENTITY_SPAWN:
      HandleEntityCreateResponse(packet);
      break;
    default:
      break;
  }
}

void PacketHandler::HandleWorldSnapshot(NetworkPacket& packet)
{
  (void)packet;
}

void PacketHandler::HandleEntityCreateResponse(NetworkPacket& packet)
{
  (void)packet;
}

} // namespace Shabby
