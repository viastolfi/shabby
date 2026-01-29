#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "networking/packet.h"

namespace engine {

class Snapshot {
public:
  explicit Snapshot();
  virtual ~Snapshot() = 0;
  virtual Packet SnapshotToPacket() = 0;
};

} // namespace engine

#endif // SNAPSHOT_H
