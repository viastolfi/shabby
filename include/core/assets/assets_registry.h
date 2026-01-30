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
  int cols = 0;
  int rows = 0;
};

struct TextureDesc {
  Texture2D texture;
  const char* path;
  int cols;
  int rows;
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
    return _textures.at(static_cast<int>(id)).texture; 
  }

  const char* GetTexturePath(T id) const
  {
    return _textures.at(static_cast<int>(id)).path; 
  }

  int GetTextureCols(T id) const 
  {
    return _textures.at(static_cast<int>(id)).cols; 
  }

  int GetTextureRows(T id) const
  {
    return _textures.at(static_cast<int>(id)).rows; 
  }
private:
  std::unordered_map<int, TextureDesc> _textures;

  void LoadOne(const AssetDesc<T>& asset)
  {
    int key = static_cast<int>(asset.id); 
    _textures.emplace(key, TextureDesc{
        LoadTexture(asset.path), 
        asset.path,
        asset.cols,
        asset.rows
      });
  }
};

} // namespace engine

#endif // ASSETS_REGISTRY_H
