#include "networking/packet_handler.h"

namespace engine {

PacketHandler::PacketHandler(
    Scene* scene, 
    size_t local_player_id, 
    const SpriteFactory* sprite_factory)
  : _scene(scene),
    _local_player_id(local_player_id),
    _sprite_factory(sprite_factory)
{}

void PacketHandler::HandlePacket(Packet& packet)
{
  switch (packet._type) {
    case PacketType::WORLD_SNAPSHOT:
      HandleWorldSnapshot(packet);
      break;
    case PacketType::ENTITY_CREATE_RESPONSE:
      HandleEntityCreateResponse(packet);
      break;
    default:
      break;
  }
}

void PacketHandler::HandleWorldSnapshot(Packet& packet)
{
  if (_scene) {
    _scene->ApplyWorldSnapshot(packet, _local_player_id);
  }
}

void PacketHandler::HandleEntityCreateResponse(Packet& packet)
{
  if (_scene && _sprite_factory && _sprite_factory->IsInitialized()) {
    auto factory_fn = [this](int texture_id) {
      return _sprite_factory->CreateSprite(texture_id);
    };
    _scene->ApplyNewEntitySnapshot(packet, _local_player_id, factory_fn);
  }
}

} // namespace engine
