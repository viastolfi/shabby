#ifndef SNAPSHOT_HANDLER_H
#define SNAPSHOT_HANDLER_H

#include "networking/protocol/ipacket_handler.h"
#include "replication/snapshot/world_snapshot.h"

namespace Shabby {

class SnapshotHandler : public IPacketHandler {
public:
  NetworkPacket* HandleFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene) override;

  void HandleFromServer(
      NetworkPacket& packet,
      Scene* scene) override;
};

} // namespace Shabby

#endif // SNAPSHOT_HANDLER_H
