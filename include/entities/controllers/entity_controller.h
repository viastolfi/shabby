#ifndef ENTITY_CONTROLLER_H
#define ENTITY_CONTROLLER_H

namespace engine {

class Entity;

class IEntityController {
public:
  virtual ~IEntityController() = 0;
  virtual void OnInit(Entity* entity) = 0;
  virtual void OnUpdate(Entity* entity, float dt) = 0;
};

} // namespace engine

#endif // ENTITY_CONTROLLER_H

