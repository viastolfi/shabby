#ifndef ASSETS_REGISTRY_H
#define ASSETS_REGISTRY_H

#include "raylib.h"

#include <unordered_map>
#include <type_traits>

namespace engine {

template<typename T>
struct AssetDesc {
  T id;
  const char* path;
};

template<typename T>
requires std::is_enum_v<T>
class AssetRegistry {
public:
  template<typename... Args>
  void LoadAll(Args&&... assets)
  {
    (LoadOne(std::forward<Args>(assets)), ...); 
  }

  Texture2D GetTexture(T id) const
  {
    return _textures.at(static_cast<int>(id)); 
  }

  const char* GetTexturePath(T id) const
  {
    return _textures_path.at(static_cast<int>(id)); 
  }
private:
  std::unordered_map<int, Texture2D> _textures;
  std::unordered_map<int, const char*> _textures_path;

  void LoadOne(const AssetDesc<T>& asset)
  {
    int key = static_cast<int>(asset.id); 
    _textures.emplace(key, LoadTexture(asset.path));
    _textures_path.emplace(key, asset.path);
  }
};

} // namespace engine

#endif // ASSETS_REGISTRY_H
