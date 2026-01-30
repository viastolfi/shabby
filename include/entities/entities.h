#ifndef ENTITIES_H
#define ENTITIES_H

#include <memory>
#include "core/sprite/sprite.h"
#include "core/sprite/animated_sprite.h"
#include "networking/client.h"

namespace engine {

class Entity {
public:
  explicit Entity(std::unique_ptr<Sprite> s, size_t id = 0);
  explicit Entity(std::unique_ptr<Sprite> s, Vector2 pos, size_t id = 0);
  explicit Entity(Vector2 pos, size_t id = 0);
  virtual ~Entity() = default;
  
  // remove copy
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;

  // accept move
  Entity(Entity&&) = default;
  Entity& operator=(Entity&&) = default;
  
  virtual void Update(float dt);
  void Draw() const;
  
  Vector2 GetPos() const;
  size_t GetId() const;
  void SetId(size_t id);
  void SetPos(Vector2 p);
  void LoadSprite() const;
  const char* GetSpritePath() const;
  int GetSpriteTextureId() const;
private:
  size_t _id;
  std::unique_ptr<Sprite> _sprite;
  int _velocity;
  Vector2 _pos;
  Client* _client = nullptr;
};

} // namespace engine

#endif // ENTITIES_H
