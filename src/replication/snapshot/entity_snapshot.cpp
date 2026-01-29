#include "replication/snapshot/entity_snapshot.h"

namespace engine {

EntitySnapshot::EntitySnapshot(size_t id, Vector2 direction)
  :_id(id), _direction(direction)
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

} // namesapce engine
