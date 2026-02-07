#include "replication/snapshot/world_snapshot.h"

namespace engine {

WorldSnapshot::WorldSnapshot() {}

WorldSnapshot::~WorldSnapshot() {}

void WorldSnapshot::AddEntity(uint64_t id, Vector2 position, int texture_id)
{
  _entities.push_back({id, position, texture_id});
}

NetworkPacket WorldSnapshot::SnapshotToPacket()
{
  NetworkPacket packet(PacketType::SNAPSHOT);
  packet.Write(static_cast<uint16_t>(_entities.size()));
  
  for (const auto& entity : _entities) {
    packet.Write(entity.id);
    packet.Write(entity.position);
    packet.Write(static_cast<uint32_t>(entity.texture_id));
  }
  
  return packet;
}

WorldSnapshot WorldSnapshot::FromPacket(NetworkPacket& packet)
{
  WorldSnapshot snapshot;
  
  uint16_t count;
  packet.Read(count);
  
  for (uint16_t i = 0; i < count; i++) {
    uint64_t id;
    Vector2 position;
    uint32_t texture_id;
    
    packet.Read(id);
    packet.Read(position);
    packet.Read(texture_id);
    
    snapshot.AddEntity(id, position, static_cast<int>(texture_id));
  }
  
  return snapshot;
}

} // namespace engine
