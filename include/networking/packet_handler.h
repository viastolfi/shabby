#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "networking/packet.h"
#include "scene/scene.h"
#include "core/factories/sprite_factory.h"

namespace engine {

class PacketHandler {
public:
  PacketHandler(Scene* scene, size_t local_player_id, const SpriteFactory* sprite_factory);
  ~PacketHandler() = default;
  
  void HandlePacket(Packet& packet);
  
private:
  Scene* _scene;
  size_t _local_player_id;
  const SpriteFactory* _sprite_factory;
  
  void HandleWorldSnapshot(Packet& packet);
  void HandleEntityCreateResponse(Packet& packet);
};

} // namespace engine

#endif // PACKET_HANDLER_H
