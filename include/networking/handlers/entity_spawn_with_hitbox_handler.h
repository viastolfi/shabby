#ifndef ENTITY_SPAWN_WITH_HITBOX_H
#define ENTITY_SPAWN_WITH_HITBOX_H

#include "networking/protocol/ipacket_handler.h"
#include "networking/protocol/network_packet.h"
#include "entities/controllers/entity_controller.h"
#include <functional>
#include <memory>

namespace engine {

using ControllerFactory = std::function<std::unique_ptr<IEntityController>(int)>;

class EntitySpawnWithHitboxHandler : public IPacketHandler {
public:
  EntitySpawnWithHitboxHandler() = default;
  explicit EntitySpawnWithHitboxHandler(ControllerFactory factory);
  ~EntitySpawnWithHitboxHandler() = default;

  NetworkPacket* HandleFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene) override;

  void HandleFromServer(
      NetworkPacket& packet,
      Scene* scene) override;

private:
  ControllerFactory _controller_factory;
};

} // namespace engine

#endif // ENTITY_SPAWN_WITH_HITBOX_H
