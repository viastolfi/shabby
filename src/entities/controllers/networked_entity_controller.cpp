#include "entities/controllers/networked_entity_controller.h"

namespace Shabby {

void NetworkedEntityController::OnInit(Entity* entity)
{
  if (!_client || !_client->IsConnected()) {
    return;
  }
  NetworkPacket request(PacketType::ENTITY_SPAWN);
  request.Write(entity->_pos);
  request.Write(static_cast<uint32_t>(entity->GetSpriteTextureId()));
  _client->Send(request);
}

void NetworkedEntityController::OnUpdate(Entity* entity, float dt) 
{
  if (!_client || !_client->IsConnected()) {
    return;
  }
  
  if (!entity->_server_id_assigned) {
    return;
  }
  
  NetworkPacket packet(PacketType::INPUT_COMMAND);
  packet.Write(static_cast<uint64_t>(entity->_id));
  packet.Write(entity->_pos);
  _client->Send(packet);
  
  (void)dt;
}

} // namespace Shabby
