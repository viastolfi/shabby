#include "networking/handlers/snapshot_handler.h"

namespace Shabby {

NetworkPacket* SnapshotHandler::HandleFromClient(
    int client_id,
    NetworkPacket& packet,
    Scene* scene)
{
  (void)client_id;
  (void)packet;
  (void)scene;
  return nullptr;
}

void SnapshotHandler::HandleFromServer(
    NetworkPacket& packet,
    Scene* scene)
{
  WorldSnapshot snapshot = WorldSnapshot::FromPacket(packet);
  scene->ApplyWorldSnapshot(snapshot);
}

} // namespace Shabby
