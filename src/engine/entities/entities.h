#ifndef ENTITIES_H
#define ENTITIES_H

#include <memory>
#include "engine/core/sprite/sprite.h"

namespace engine {

class Entity {
public:
  explicit Entity(const char* texture_path);
  virtual ~Entity() = default;
  
  // remove copy
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;

  // accept move
  Entity(Entity&&) = default;
  Entity& operator=(Entity&&) = default;
  
  virtual void Update(float dt);
  virtual void Draw() const;

  Vector2 GetPos();

protected:
  void SetPos(Vector2 p);
  
private:
  std::unique_ptr<Sprite> _sprite;
  int _velocity;
  Vector2 _pos;
};

} // namespace engine

#endif // ENTITIES_H
