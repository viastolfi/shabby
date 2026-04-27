#ifndef TILE_MAP_NODE_H
#define TILE_MAP_NODE_H

#include <memory>
#include <unordered_set>
#include <vector>

#include "node/inode.h"
#include "core/drawable/idrawable.h"
#include "core/drawable/ibounded.h"
#include "core/physics/icollider.h"
#include "core/tileset/tileset.h"

#include "raylib.h"

namespace Shabby::Core { class CollisionSystem; }

namespace Shabby::Node {

class TileMapNode : public INode, public Core::IDrawable, public Core::IBounded {
public:
  TileMapNode(Vector2 pos, Core::Tileset tileset, int map_cols, int map_rows);
  ~TileMapNode();

  void SetTile(int col, int row, int tile_id);
  int  GetTile(int col, int row) const;
  void Fill(int tile_id);
  void LoadData(std::vector<int> data);

  void MarkSolidTile(int tile_id);
  void SetTileCollisionLayer(int layer);
  void RegisterColliders(Core::CollisionSystem* cs);
  void UnregisterColliders();

  void      Draw()      override;
  void      Update(float dt) override;
  Rectangle GetBounds() const override;

private:
  Core::Tileset    _tileset;
  int              _map_cols;
  int              _map_rows;
  std::vector<int> _tiles;

  std::unordered_set<int>                       _solid_tile_ids;
  std::vector<std::unique_ptr<Core::ICollider>> _tile_colliders;
  Core::CollisionSystem*                        _collision_system     = nullptr;
  int                                           _tile_collision_layer = 1;
};

} // namespace Shabby::Node

#endif // TILE_MAP_NODE_H
