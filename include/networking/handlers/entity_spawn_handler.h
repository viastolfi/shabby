#ifndef ENTITY_SPAWN_HANDLER_H
#define ENTITY_SPAWN_HANDLER_H

#include "networking/protocol/ipacket_handler.h"
#include "entities/controllers/entity_controller.h"
#include "raylib.h"
#include <sstream>
#include <functional>
#include <memory>

namespace Shabby {

using ControllerFactory = std::function<std::unique_ptr<IEntityController>(int)>;

class EntitySpawnHandler : public IPacketHandler {
public:
  EntitySpawnHandler() = default;
  explicit EntitySpawnHandler(ControllerFactory factory);
  ~EntitySpawnHandler() = default;
  
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

} // namespace Shabby 

#endif // ENTITY_SPAWN_HANDLER_H
