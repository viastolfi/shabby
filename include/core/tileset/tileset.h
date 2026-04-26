#ifndef TILESET_H
#define TILESET_H

#include "raylib.h"

namespace Shabby::Core {

class Tileset {
public:
  Tileset(Texture2D texture, int tile_width, int tile_height, int margin = 0)
    : _texture(texture),
      _tile_width(tile_width),
      _tile_height(tile_height),
      _margin(margin),
      _cols(texture.width  / tile_width),
      _rows(texture.height / tile_height)
  {}

  Rectangle GetTileRect(int tile_id) const
  {
    int col = tile_id % _cols;
    int row = tile_id / _cols;
    return {
      (float)(col * _tile_width  + _margin),
      (float)(row * _tile_height + _margin),
      (float)(_tile_width  - 2 * _margin),
      (float)(_tile_height - 2 * _margin)
    };
  }

  int       GetTileWidth()  const { return _tile_width;        }
  int       GetTileHeight() const { return _tile_height;       }
  int       GetTileCount()  const { return _cols * _rows;      }
  int       GetMargin()     const { return _margin;            }
  Texture2D GetTexture()    const { return _texture;           }

private:
  Texture2D _texture;
  int       _tile_width;
  int       _tile_height;
  int       _margin;
  int       _cols;
  int       _rows;
};

} // namespace Shabby::Core

#endif // TILESET_H
