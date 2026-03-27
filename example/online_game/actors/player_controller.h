#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "actors/player_controller_base.h"
#include "entities/controllers/networked_entity_controller.h"
#include "entities/entity.h"
#include "raymath.h"

class PlayerController : public PlayerControllerBase {
public:
  explicit PlayerController(engine::Client* client);
  ~PlayerController() = default;

  void OnInit(engine::Entity* entity) override;
  void OnUpdate(engine::Entity* entity, float dt) override;

private:
  std::unique_ptr<engine::NetworkedEntityController> _network;
};

#endif // PLAYER_CONTROLLER_H
