#ifndef PACKET_REGISTRY_H
#define PACKET_REGISTRY_H

#include "networking/protocol/ipacket_handler.h"
#include "networking/protocol/packet_type.h"
#include <unordered_map>
#include <memory>

namespace Shabby {

class PacketRegistry {
public:
  PacketRegistry() = default;
  ~PacketRegistry() = default;
  
  void RegisterHandler(PacketType type, std::unique_ptr<IPacketHandler> handler);
  
  NetworkPacket* RouteFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene);
  
  void RouteFromServer(
      NetworkPacket& packet,
      Scene* scene);
  
  bool HasHandler(PacketType type) const;
private:
  std::unordered_map<PacketType, std::unique_ptr<IPacketHandler>> _handlers;
};

} // namespace Shabby

#endif // PACKET_REGISTRY_H
