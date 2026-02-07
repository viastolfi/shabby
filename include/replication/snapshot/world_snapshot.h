#ifndef WORLD_SNAPSHOT_H
#define WORLD_SNAPSHOT_H

#include "replication/snapshot/snapshot.h"
#include "entities/entity.h"
#include <vector>

namespace engine {

class WorldSnapshot : public Snapshot {
public:
  WorldSnapshot();
  ~WorldSnapshot();

  void AddEntity(uint64_t id, Vector2 position, int texture_id);
  NetworkPacket SnapshotToPacket() override;
  
  static WorldSnapshot FromPacket(NetworkPacket& packet);
  
  struct EntityData {
    uint64_t id;
    Vector2 position;
    int texture_id;
  };
  
  const std::vector<EntityData>& GetEntities() const { return _entities; }

private:
  std::vector<EntityData> _entities;
};

} // namespace engine

#endif // WORLD_SNAPSHOT_H
