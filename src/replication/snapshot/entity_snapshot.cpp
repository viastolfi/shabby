#include "replication/snapshot/entity_snapshot.h"

namespace engine {

EntitySnapshot::EntitySnapshot(
    size_t id, Vector2 direction, int texture_id)
  :_id(id), _direction(direction), _texture_id(texture_id)
{}

EntitySnapshot::~EntitySnapshot() 
{}

Packet EntitySnapshot::SnapshotToPacket() 
{
  Packet p(PacketType::ENTITY_SNAPSHOT);

  p.Write(_id);
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

} // namesapce engine
