#ifndef ENTITIES_H
#define ENTITIES_H

#include <memory>
#include <optional>
#include <functional>
#include "core/sprite/sprite.h"
#include "core/sprite/animated_sprite.h"
#include "networking/client.h"
#include "entities/controllers/entity_controller.h"
#include "physics/hitbox/hitbox.h"
#include "utils/raylog.h"

namespace engine {

class Entity {
public:
  explicit Entity();
  ~Entity(); 
  
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;
  Entity(Entity&&) = default;
  Entity& operator=(Entity&&) = default;
  
  void Draw() const;
  void Init();
  void Update(float dt);
  void OnHitboxEntered(Hitbox* enter, Hitbox* from);
  void OnHitboxExited(Hitbox* enter, Hitbox* from);

  void LoadSprite() const;
  const char* GetSpritePath() const;
  int GetSpriteTextureId() const;
  const Texture2D* GetTexture() const;
  std::optional<std::reference_wrapper<const Rectangle>>
    GetFrameRec() const;
  const std::function<Hitbox*()> GetHitboxCreateFunction() const;
  Hitbox* GetHitbox() const;

  void SetController(std::unique_ptr<IEntityController> c);
  void SetVelocity(int velocity);
  void SetSprite(std::unique_ptr<Sprite> sprite);
  void SetHitbox(Hitbox* hitbox);
  void SetHitboxCreationFunction(std::function<Hitbox*()> func);

  size_t _id = 0;
  Vector2 _pos = {0.f, 0.f};
  bool _is_local = false;
  bool _server_id_assigned = false;
  int _texture_id = 0;
private:
  int _velocity = 0;
  std::unique_ptr<Sprite> _sprite;
  std::unique_ptr<IEntityController> _controller;
  Hitbox* _hitbox = nullptr;
  std::function<Hitbox*()> _hitbox_creation_function;
protected:
};

} // namespace engine

#endif // ENTITIES_H
