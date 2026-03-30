#include "networking/handlers/input_command_handler.h"

namespace Shabby {

NetworkPacket* InputCommandHandler::HandleFromClient(
    int client_id,
    NetworkPacket& packet,
    Scene* scene)
{
  uint64_t entity_id;
  Vector2 position;
  
  packet.Read(entity_id);
  packet.Read(position);
  
  for (auto* entity : scene->GetEntities()) {
    if (entity->_id == entity_id) {
      entity->_pos = position;
      break;
    }
  }
  
  (void)client_id;
  return nullptr;
}

void InputCommandHandler::HandleFromServer(
    NetworkPacket& packet,
    Scene* scene)
{
  (void)packet;
  (void)scene;
}

} // namespace Shabby
