#ifndef ASSETS_REGISTRY_H
#define ASSETS_REGISTRY_H

#include "raylib.h"

#include <unordered_map>
#include <type_traits>

namespace Shabby::Core {

struct AssetDesc {
  int id;
  const char* path;
  int cols = -1;
  int rows = -1;
};

struct TextureDesc {
  Texture2D texture;
  const char* path;
  int cols;
  int rows;
};

class AssetRegistry {
public:
  template<typename... Args>
  void LoadAll(Args&&... assets)
  {
    (LoadOne(std::forward<Args>(assets)), ...); 
  }

  Texture2D GetTexture(int id) const
  {
    return _textures.at(id).texture;
  }

  const char* GetTexturePath(int id) const
  {
    return _textures.at(id).path; 
  }

  int GetTextureCols(int id) const 
  {
    return _textures.at(id).cols; 
  }

  int GetTextureRows(int id) const
  {
    return _textures.at(id).rows; 
  }

  TextureDesc GetTextureDesc(int id) const 
  {
    return _textures.at(id);
  }
  
private:
  std::unordered_map<int, TextureDesc> _textures;

  void LoadOne(const AssetDesc& asset)
  {
    int key = asset.id; 
    _textures.emplace(key, TextureDesc{
        LoadTexture(asset.path), 
        asset.path,
        asset.cols,
        asset.rows
      });
  }
};

} // namespace Shabby::Core

#endif // ASSETS_REGISTRY_H
