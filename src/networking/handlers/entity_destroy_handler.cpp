#include "networking/handlers/entity_destroy_handler.h"

namespace Shabby {

NetworkPacket* EntityDestroyHandler::HandleFromClient(
    int client_id,
    NetworkPacket& packet,
    Scene* scene)
{
  (void)client_id;
  (void)packet;
  (void)scene;
  return nullptr;
}

void EntityDestroyHandler::HandleFromServer(
    NetworkPacket& packet,
    Scene* scene)
{
  uint64_t entity_id;
  packet.Read(entity_id);
  
  scene->RemoveEntity(entity_id);
}

} // namespace Shabby
