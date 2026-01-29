#ifndef ENTITY_SNAPSHOT_H
#define ENTITY_SNAPSHOT_H

#include "raylib.h"
#include "replication/snapshot/snapshot.h"

namespace engine {

class EntitySnapshot : public Snapshot {
public:
  explicit EntitySnapshot(size_t id, Vector2 direction);
  ~EntitySnapshot();

  Packet SnapshotToPacket() override;

  size_t GetId() const;
  Vector2 GetDirection() const;
private:
  size_t _id;
  Vector2 _direction;
};

} // namespace engine

#endif // ENTITY_SNAPSHOT_H
