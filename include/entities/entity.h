#ifndef ENTITIES_H
#define ENTITIES_H

#include <memory>
#include "core/sprite/sprite.h"
#include "core/sprite/animated_sprite.h"
#include "networking/client.h"
#include "entities/controllers/entity_controller.h"

namespace engine {

class Entity {
public:
  explicit Entity();
  virtual ~Entity() = default;
  
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;
  Entity(Entity&&) = default;
  Entity& operator=(Entity&&) = default;
  
  void Draw() const;
  void Init();
  void Update(float dt);

  void LoadSprite() const;
  const char* GetSpritePath() const;
  int GetSpriteTextureId() const;

  void SetController(std::unique_ptr<IEntityController> c);
  void SetVelocity(int velocity);
  void SetSprite(std::unique_ptr<Sprite> sprite);

  size_t _id = 0;
  Vector2 _pos = {0.f, 0.f};
  bool _is_local = false;
  bool _server_id_assigned = false;
  int _texture_id = 0;
private:
  int _velocity = 0;
  std::unique_ptr<Sprite> _sprite;
  std::unique_ptr<IEntityController> _controller;
  std::unique_ptr<Hitbox> _hitbox;
protected:
};

} // namespace engine

#endif // ENTITIES_H
