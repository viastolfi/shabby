#include "replication/snapshot/entity_snapshot.h"

namespace Shabby {

EntitySnapshot::EntitySnapshot(
    size_t id, Vector2 direction, int texture_id)
  :_id(id), _direction(direction), _texture_id(texture_id)
{}

EntitySnapshot::~EntitySnapshot() 
{}

NetworkPacket EntitySnapshot::SnapshotToPacket() 
{
  NetworkPacket p(PacketType::SNAPSHOT);

  p.Write(static_cast<uint64_t>(_id));
  p.Write(_direction);

  return p;
}

size_t EntitySnapshot::GetId() const 
{
  return _id;
}

Vector2 EntitySnapshot::GetDirection() const
{
  return _direction; 
}

int EntitySnapshot::GetTextureId() const 
{
  return _texture_id;
}

} // namespace Shabby
