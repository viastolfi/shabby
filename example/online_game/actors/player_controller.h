#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "actors/player_controller_base.h"
#include "entities/controllers/networked_entity_controller.h"
#include "entities/entity.h"
#include "raymath.h"

class PlayerController : public PlayerControllerBase {
public:
  explicit PlayerController(Shabby::Client* client);
  ~PlayerController() = default;

  void OnInit(Shabby::Entity* entity) override;
  void OnUpdate(Shabby::Entity* entity, float dt) override;

private:
  std::unique_ptr<Shabby::NetworkedEntityController> _network;
};

#endif // PLAYER_CONTROLLER_H
