#include "node/tilemap/tile_map_node.h"

#include <cassert>
#include <algorithm>

namespace Shabby::Node {

TileMapNode::TileMapNode(Vector2 pos, Core::Tileset tileset, int map_cols, int map_rows)
  : INode(pos),
    _tileset(tileset),
    _map_cols(map_cols),
    _map_rows(map_rows),
    _tiles(map_cols * map_rows, -1)
{}

void TileMapNode::SetTile(int col, int row, int tile_id)
{
  assert(col >= 0 && col < _map_cols);
  assert(row >= 0 && row < _map_rows);
  assert(tile_id == -1 || (tile_id >= 0 && tile_id < _tileset.GetTileCount()));
  _tiles[row * _map_cols + col] = tile_id;
}

int TileMapNode::GetTile(int col, int row) const
{
  assert(col >= 0 && col < _map_cols);
  assert(row >= 0 && row < _map_rows);
  return _tiles[row * _map_cols + col];
}

void TileMapNode::Fill(int tile_id)
{
  assert(tile_id == -1 || (tile_id >= 0 && tile_id < _tileset.GetTileCount()));
  std::fill(_tiles.begin(), _tiles.end(), tile_id);
}

void TileMapNode::LoadData(std::vector<int> data)
{
  assert((int)data.size() == _map_cols * _map_rows);
  _tiles = std::move(data);
}

void TileMapNode::Draw()
{
  float     tw  = (float)_tileset.GetTileWidth();
  float     th  = (float)_tileset.GetTileHeight();
  Texture2D tex = _tileset.GetTexture();

  // TODO: compute visible tile range from active camera to skip off-screen tiles
  for (int row = 0; row < _map_rows; ++row) {
    for (int col = 0; col < _map_cols; ++col) {
      int tile_id = _tiles[row * _map_cols + col];
      if (tile_id < 0) continue;

      Rectangle src  = _tileset.GetTileRect(tile_id);
      Rectangle dest = { _pos.x + col * tw, _pos.y + row * th, tw, th };
      DrawTexturePro(tex, src, dest, { 0, 0 }, 0.0f, WHITE);
    }
  }
}

void TileMapNode::Update(float dt)
{
  INode::Update(dt);
}

Rectangle TileMapNode::GetBounds() const
{
  return {
    _pos.x, _pos.y,
    (float)(_map_cols * _tileset.GetTileWidth()),
    (float)(_map_rows * _tileset.GetTileHeight())
  };
}

} // namespace Shabby::Node
