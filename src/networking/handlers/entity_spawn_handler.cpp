#include "networking/handlers/entity_spawn_handler.h"

namespace Shabby {

EntitySpawnHandler::EntitySpawnHandler(ControllerFactory factory)
  : _controller_factory(std::move(factory))
{}

NetworkPacket* EntitySpawnHandler::HandleFromClient(
    int client_id,
    NetworkPacket& packet,
    Scene* scene)
{
  Vector2 position;
  uint32_t texture_id;
  packet.Read(position);
  packet.Read(texture_id);

  std::unique_ptr<IEntityController> controller = nullptr;
  if (_controller_factory) {
    controller = _controller_factory(static_cast<int>(texture_id));
  }

  Entity* entity = scene->AddEntity(
    std::move(controller),
    static_cast<int>(texture_id)
  );
  
  entity->_pos = position;

  NetworkPacket* response = new NetworkPacket(PacketType::ENTITY_SPAWN);
  response->Write(static_cast<uint64_t>(entity->_id));
  response->Write(entity->_pos);
  response->Write(static_cast<uint32_t>(texture_id));
  
  (void)client_id;
  return response;
}

void EntitySpawnHandler::HandleFromServer(
    NetworkPacket& packet,
    Scene* scene)
{
  uint64_t entity_id;
  Vector2 position;
  uint32_t texture_id;
  
  packet.Read(entity_id);
  packet.Read(position);
  packet.Read(texture_id);

  bool found = false;
  for (auto* entity : scene->GetEntities()) {
    if (entity->_is_local && !entity->_server_id_assigned) {
      entity->_id = entity_id;
      entity->_server_id_assigned = true;
      found = true;
      break;
    }
  }
  
  if (!found) {
    scene->AddEntity(entity_id, static_cast<int>(texture_id), position);
  }
}

} // namespace Shabby
