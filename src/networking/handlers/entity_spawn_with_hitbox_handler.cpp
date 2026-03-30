#include "networking/handlers/entity_spawn_with_hitbox_handler.h"

namespace Shabby {

EntitySpawnWithHitboxHandler::EntitySpawnWithHitboxHandler(
    ControllerFactory factory)
  : _controller_factory(std::move(factory))
{}

NetworkPacket* EntitySpawnWithHitboxHandler::HandleFromClient(
    int client_id,
    NetworkPacket& packet,
    Scene* scene) 
{
  Vector2 position;
  uint32_t texture_id;
  float x, y, width, height;

  packet.Read(position);
  packet.Read(texture_id);
  packet.Read(x);
  packet.Read(y);
  packet.Read(width);
  packet.Read(height);

  std::unique_ptr<IEntityController> controller = nullptr;
  if (_controller_factory) {
    controller = _controller_factory(static_cast<int>(texture_id));
  }

  Entity* entity = scene->AddEntity(
      std::move(controller),
      static_cast<int>(texture_id),
      x, y, width, height);

  entity->_pos = position;

  NetworkPacket* response = 
    new NetworkPacket(PacketType::ENTITY_SPAWN);

  response->Write(static_cast<uint64_t>(entity->_id));
  response->Write(entity->_pos);
  response->Write(static_cast<uint32_t>(texture_id));

  (void)client_id;
  return response;
}

void EntitySpawnWithHitboxHandler::HandleFromServer(
    NetworkPacket& packet,
    Scene* scene)
{
  (void)packet;
  (void)scene;
} 

} // namespace Shabby
