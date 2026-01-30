#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H

#include <memory>
#include <functional>
#include "core/sprite/sprite.h"

namespace engine {

class SpriteFactory {
public:
  SpriteFactory() = default;
  ~SpriteFactory() = default;
  
  void SetFactory(std::function<std::unique_ptr<Sprite>(int)> factory) 
  {
    _factory = factory;
  }
  
  std::unique_ptr<Sprite> CreateSprite(int texture_id) const 
  {
    if (_factory) {
      return _factory(texture_id);
    }
    return nullptr;
  }
  
  bool IsInitialized() const 
  {
    return _factory != nullptr;
  }
  
private:
  std::function<std::unique_ptr<Sprite>(int)> _factory;
};

} // namespace engine

#endif // SPRITE_FACTORY_H
