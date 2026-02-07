#include "networking/protocol/packet_registry.h"

namespace engine {

void PacketRegistry::RegisterHandler(
    PacketType type,
    std::unique_ptr<IPacketHandler> handler)
{
  _handlers[type] = std::move(handler);
}

NetworkPacket* PacketRegistry::RouteFromClient(
    int client_id,
    NetworkPacket& packet,
    Scene* scene)
{
  auto it = _handlers.find(packet.GetType());
  if (it != _handlers.end()) {
    return it->second->HandleFromClient(client_id, packet, scene);
  } else {
    // TODO: add exception handling
  }
}

void PacketRegistry::RouteFromServer(
    NetworkPacket& packet,
    Scene* scene)
{
  auto it = _handlers.find(packet.GetType());
  if (it != _handlers.end()) {
    it->second->HandleFromServer(packet, scene);
  } else {
    // TODO: add exception handling
  }
}

bool PacketRegistry::HasHandler(PacketType type) const
{
  return _handlers.find(type) != _handlers.end();
}

} // namespace engine
