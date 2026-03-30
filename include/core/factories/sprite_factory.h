#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H

#include <memory>
#include <typeindex>
#include <stdexcept>
#include "core/sprite/sprite.h"
#include "core/assets/assets_registry.h"

namespace Shabby {

class SpriteFactory {
public:
  SpriteFactory(AssetRegistry* assets_registry)
    :_assets_registry(assets_registry)
  {}

  ~SpriteFactory() = default;
  
  std::shared_ptr<Sprite> CreateSprite(int texture_id) const 
  {
    if (!_assets_registry || texture_id < 0)
      return nullptr;
  
    try {
      TextureDesc desc = _assets_registry->GetTextureDesc(texture_id);    
      if (desc.rows == -1) {
        auto sprite = std::make_shared<Sprite>(
            desc.texture, texture_id, desc.path);
        sprite->SetOwnsTexture(false);
        return sprite;
      } else {
        auto sprite = std::make_shared<AnimatedSprite>(
            desc.texture, 
            texture_id, 
            desc.path, 
            desc.cols, 
            desc.rows);
        sprite->SetOwnsTexture(false);
        return sprite;
      }
    } catch (const std::out_of_range&) {
      return nullptr;
    }
  }
private:
  AssetRegistry* _assets_registry;
};

} // namespace Shabby

#endif // SPRITE_FACTORY_H
