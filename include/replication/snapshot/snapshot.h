#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "networking/protocol/network_packet.h"

namespace Shabby {

class Snapshot {
public:
  explicit Snapshot();
  virtual ~Snapshot() = 0;
  virtual NetworkPacket SnapshotToPacket() = 0;
};

} // namespace Shabby

#endif // SNAPSHOT_H
