#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <memory>
#include "core/factories/sprite_factory.h"
#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"

namespace Shabby {

class EntityFactory {
public:
  EntityFactory(std::unique_ptr<SpriteFactory> sprite_factory)
    : _sprite_factory(std::move(sprite_factory))
  {}

  Entity* CreateEntity(
      std::unique_ptr<IEntityController> controller,
      int texture_id)
  {
    std::shared_ptr<Sprite> sprite = 
      _sprite_factory->CreateSprite(texture_id);
    Entity* e = new Entity();
    e->SetSprite(std::move(sprite));
    e->SetController(std::move(controller));
    return e;
  }

  Entity* CreateEntity(
      std::unique_ptr<IEntityController> controller,
      std::unique_ptr<AnimationPlayer> ap)
  {
    Entity* e = new Entity();
    e->SetController(std::move(controller));
    e->SetAnimationPlayer(std::move(ap));
    return e;
  }
private:
  std::unique_ptr<SpriteFactory> _sprite_factory;
};

} // namespace Shabby

#endif // ENTITY_FACTORY_H
