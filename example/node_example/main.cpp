#include <memory>

#include "core/engine/engine.h"
#include "node/sprite/sprite.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "node/camera/camera.h"
#include "node/timer.h"
#include "node/tilemap/tile_map_node.h"
#include "utils/raylog.h"

#include "main_scene.h"
#include "background.h"
#include "player.h"
#include "ennemy.h"
#include "static_entity.h"

enum class AssetId {
  BEAF = 0,
  MONKEY_IDLE,
  MONKEY_WALK,
  FIELD_TILESET,
  NATURE_TILESET
};

// Positions spread across the world, outside the initial 800x600 view
// (player starts at {100,100}, initial viewport covers roughly {-300,-200} to {500,400})
static const Vector2 ENNEMY_SPAWNS[] = {
  {  800,  100 },   // far right
  {  100,  700 },   // far below
  { -400,  200 },   // far left
  {  500, -300 },   // far above
  {  900,  700 },   // bottom-right corner
};
static constexpr int ENNEMY_COUNT = 5;

int main(void)
{
  Raylog::GetInstance(1).Log(1, "Logger Set");

  // --- Engine setup ---
  auto engine        = std::make_unique<Shabby::Core::Engine>();
  auto render_system = engine->GetRenderSystem();
  auto collision_system = engine->GetCollisionSystem();

  auto assets_registry = std::make_unique<Shabby::Core::AssetRegistry>();
  assets_registry->LoadAll(
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::BEAF),          "assets/Beaf.png" },
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::MONKEY_IDLE),   "assets/actors/monkey/Idle.png" },
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::MONKEY_WALK),   "assets/actors/monkey/Walk.png" },
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::FIELD_TILESET),   "assets/tilesets/TilesetField.png" },
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::NATURE_TILESET), "assets/tilesets/TilesetNature.png" }
  );

  // --- Scene & player ---
  auto ms = std::make_shared<MainScene>();
  auto p  = std::make_shared<Player>((Vector2){ 100, 100 });

  auto player_hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
    (Rectangle){ 100, 100, 16, 16 }
  );

  player_hitbox->entered.connect(
    [p](
      Shabby::Node::RectangleHitbox* hit, 
      Shabby::Core::ICollider* from) 
    {
      p->_on_hitbox_entered(hit, from);
    }
  );

  player_hitbox->exited.connect(
    [p](
      Shabby::Node::RectangleHitbox* hit,
      Shabby::Core::ICollider* from)
    {
      p->_on_hitbox_exited(hit, from); 
    }
  );
  player_hitbox->SetCollisionLayer(1);
  player_hitbox->AddMaskLayer(2);
  player_hitbox->AddMaskLayer(3);
  collision_system->Register(player_hitbox.get());

  // --- Animations ---
  auto idle       = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_IDLE)),
    0, 3, 0, 0, 4, 1, 3.0f
  );
  auto walk_down  = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    0, 0, 0, 3, 4, 4, 3.0f
  );
  auto walk_up    = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    1, 1, 0, 3, 4, 4, 3.0f
  );
  auto walk_left  = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    2, 2, 0, 3, 4, 4, 3.0f
  );
  auto walk_right = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    3, 3, 0, 3, 4, 4, 3.0f
  );

  auto animation_player = std::make_shared<Shabby::Node::AnimationPlayer>();
  animation_player->Register("idle",       idle);
  animation_player->Register("walk_up",    walk_up);
  animation_player->Register("walk_down",  walk_down);
  animation_player->Register("walk_left",  walk_left);
  animation_player->Register("walk_right", walk_right);
  animation_player->Play("idle");

  // --- Background fill (hides transparent tile borders) ---
  auto bg = std::make_shared<Background>();
  bg->SetRenderLayer(-11);
  render_system->Register(bg.get());

  // --- Tilemap ---
  // Tileset TilesetField.png: 16x16 px per tile, 5 cols x 15 rows (75 tiles)
  // Each terrain type spans 3 rows. Col 1 of the middle row = 100% opaque fill tile:
  //   tile  6 : sandy fill       tile 21 : light green fill
  //   tile 36 : dark green fill  tile 51 : peach fill
  static const int MAP_COLS = 130;
  static const int MAP_ROWS = 90;
  static const int TILE_W   = 16;
  static const int TILE_H   = 16;

  static const int SANDY_FILL       =  6;
  static const int LIGHT_GRASS_FILL = 21;
  static const int DARK_GRASS_FILL  = 36;
  static const int FLOWER_FILL      = 51;

  std::vector<int> tile_data(MAP_COLS * MAP_ROWS, DARK_GRASS_FILL);

  auto fill_rect = [&](int col, int row, int w, int h, int id) {
    for (int r = row; r < row + h && r < MAP_ROWS; ++r)
      for (int c = col; c < col + w && c < MAP_COLS; ++c)
        tile_data[r * MAP_COLS + c] = id;
  };

  auto fill_rect_if = [&](int col, int row, int w, int h, int id, int only_if) {
    for (int r = row; r < row + h && r < MAP_ROWS; ++r)
      for (int c = col; c < col + w && c < MAP_COLS; ++c)
        if (tile_data[r * MAP_COLS + c] == only_if)
          tile_data[r * MAP_COLS + c] = id;
  };

  // Horizontal sandy paths (3 tiles wide, slight wave)
  for (int c = 0; c < MAP_COLS; ++c) {
    int wave = (c / 8) % 2;
    fill_rect(c, 20 + wave, 1, 3, SANDY_FILL);
    fill_rect(c, 58 + wave, 1, 3, SANDY_FILL);
  }
  // Vertical sandy path
  for (int r = 0; r < MAP_ROWS; ++r) {
    int wave = (r / 7) % 2;
    fill_rect(60 + wave, r, 3, 1, SANDY_FILL);
  }

  // Light grass patches (broader open areas)
  fill_rect(10, 25, 18, 12, LIGHT_GRASS_FILL);
  fill_rect(75, 10, 20, 15, LIGHT_GRASS_FILL);
  fill_rect(40, 65, 15, 14, LIGHT_GRASS_FILL);
  fill_rect(90, 55, 22, 18, LIGHT_GRASS_FILL);
  fill_rect( 5, 68, 14, 10, LIGHT_GRASS_FILL);

  // Dense vegetation in corners and clusters
  fill_rect(  0,  0, 8, 7,   DARK_GRASS_FILL);
  fill_rect(122,  0, 8, 8,   DARK_GRASS_FILL);
  fill_rect(  0, 82, 9, 8,   DARK_GRASS_FILL);
  fill_rect(121, 82, 9, 8,   DARK_GRASS_FILL);
  fill_rect( 32,  8, 6, 6,   DARK_GRASS_FILL);
  fill_rect( 95, 38, 7, 5,   DARK_GRASS_FILL);
  fill_rect( 15, 50, 5, 7,   DARK_GRASS_FILL);
  fill_rect( 72, 72, 8, 6,   DARK_GRASS_FILL);

  // Flower patches (only on light grass or dark grass, not paths)
  fill_rect_if( 14, 27, 4, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
  fill_rect_if( 80, 12, 5, 4, FLOWER_FILL, LIGHT_GRASS_FILL);
  fill_rect_if( 44, 68, 4, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
  fill_rect_if( 96, 60, 5, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
  fill_rect_if(  8, 70, 3, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
  fill_rect_if( 20,  5, 3, 3, FLOWER_FILL, DARK_GRASS_FILL);
  fill_rect_if( 50, 40, 4, 3, FLOWER_FILL, DARK_GRASS_FILL);
  fill_rect_if(108, 28, 3, 4, FLOWER_FILL, DARK_GRASS_FILL);

  Shabby::Core::Tileset tileset{
    assets_registry->GetTexture(static_cast<int>(AssetId::FIELD_TILESET)),
    TILE_W, TILE_H
  };

  auto tilemap = std::make_shared<Shabby::Node::TileMapNode>(
    Vector2{ -500.f, -432.f }, tileset, MAP_COLS, MAP_ROWS
  );
  tilemap->LoadData(tile_data);
  tilemap->SetRenderLayer(-10);
  render_system->Register(tilemap.get());

  // --- Nature tilemap (trees, rocks, stumps) ---
  // TilesetNature.png: 24 cols × 21 rows of 16×16 tiles (RGBA with alpha)
  //
  // Big trees (4×3): rows 3–5  — top/mid rows are 4 tiles wide, feet row is 2 tiles wide at +1,+2
  //   Type A dark green : top={72..75}  mid={96..99}   feet={121,122}
  //   Type B med green  : top={76..79}  mid={100..103} feet={125,126}
  //   Type C snow/white : top={80..83}  mid={104..107} feet={129,130}
  //   Type D pink       : top={84..87}  mid={108..111} feet={133,134}
  //   Type E bright grn : top={88..91}  mid={112..115} feet={137,138}
  // Small trees (2×2): rows 0–1
  //   Pine   : {0,1,24,25}    Dead : {2,3,26,27}
  //   Pink   : {8,9,32,33}    LtGreen: {10,11,34,35}
  // Rocks (4×2): rows 6–7
  //   Brown  : top={156..159} bot={180..183}
  //   Gray   : top={160..163} bot={184..187}
  // Stumps / bushes (2×2): rows 8–9
  //   Stump  : {192,193,216,217}   Green bush: {198,199,222,223}

  Shabby::Core::Tileset nature_tileset{
    assets_registry->GetTexture(static_cast<int>(AssetId::NATURE_TILESET)),
    16, 16
  };

  auto nature_map = std::make_shared<Shabby::Node::TileMapNode>(
    Vector2{ -500.f, -432.f }, nature_tileset, MAP_COLS, MAP_ROWS
  );

  std::vector<int> nature_data(MAP_COLS * MAP_ROWS, -1);

  auto nt = [&](int col, int row, int id) {
    if (col >= 0 && col < MAP_COLS && row >= 0 && row < MAP_ROWS)
      nature_data[row * MAP_COLS + col] = id;
  };

  auto big_tree = [&](int c, int r,
    int t0, int t1, int t2, int t3,
    int m0, int m1, int m2, int m3,
    int f1, int f2)
  {
    nt(c,   r,   t0); nt(c+1, r,   t1); nt(c+2, r,   t2); nt(c+3, r,   t3);
    nt(c,   r+1, m0); nt(c+1, r+1, m1); nt(c+2, r+1, m2); nt(c+3, r+1, m3);
    nt(c+1, r+2, f1); nt(c+2, r+2, f2);
  };

  auto sm_tree = [&](int c, int r, int tl, int tr, int bl, int br) {
    nt(c, r, tl); nt(c+1, r, tr);
    nt(c, r+1, bl); nt(c+1, r+1, br);
  };

  auto rocks = [&](int c, int r,
    int r0c0, int r0c1, int r0c2, int r0c3,
    int r1c0, int r1c1, int r1c2, int r1c3)
  {
    nt(c,   r,   r0c0); nt(c+1, r,   r0c1); nt(c+2, r,   r0c2); nt(c+3, r,   r0c3);
    nt(c,   r+1, r1c0); nt(c+1, r+1, r1c1); nt(c+2, r+1, r1c2); nt(c+3, r+1, r1c3);
  };

  // Forest cluster 1 — top-left corner (cols 1–25, rows 2–15)
  big_tree( 3,  2, 72,73,74,75, 96,97,98,99, 121,122);
  big_tree( 8,  3, 76,77,78,79, 100,101,102,103, 125,126);
  big_tree(14,  2, 84,85,86,87, 108,109,110,111, 133,134);
  big_tree(19,  4, 72,73,74,75, 96,97,98,99, 121,122);
  sm_tree( 6,  8, 0,1,24,25);
  sm_tree(11,  9, 10,11,34,35);
  sm_tree(17,  7, 8,9,32,33);
  sm_tree(22,  3, 0,1,24,25);
  sm_tree( 2,  7, 10,11,34,35);

  // Forest cluster 2 — top-right (cols 70–92, rows 2–10)
  big_tree(70,  2, 80,81,82,83, 104,105,106,107, 129,130);
  big_tree(75,  3, 84,85,86,87, 108,109,110,111, 133,134);
  big_tree(80,  2, 88,89,90,91, 112,113,114,115, 137,138);
  big_tree(86,  4, 76,77,78,79, 100,101,102,103, 125,126);
  sm_tree(73,  7, 10,11,34,35);
  sm_tree(78,  8, 0,1,24,25);
  sm_tree(84,  7, 8,9,32,33);

  // Forest cluster 3 — bottom strip (below lower path at row ~58)
  big_tree( 5, 67, 72,73,74,75, 96,97,98,99, 121,122);
  big_tree(12, 68, 88,89,90,91, 112,113,114,115, 137,138);
  big_tree(45, 66, 84,85,86,87, 108,109,110,111, 133,134);
  sm_tree(10, 73, 0,1,24,25);
  sm_tree( 3, 73, 10,11,34,35);

  // Forest cluster 4 — far right past vertical path (cols 99–111, rows 63–69)
  big_tree(100, 65, 76,77,78,79, 100,101,102,103, 125,126);
  big_tree(106, 66, 72,73,74,75, 96,97,98,99, 121,122);

  // Rock formations — right-of-centre area (player detects these on collision layer 2)
  rocks(93, 38, 156,157,158,159, 180,181,182,183);
  rocks(99, 40, 156,157,158,159, 180,181,182,183);
  rocks(106, 36, 160,161,162,163, 184,185,186,187);
  rocks(110, 42, 160,161,162,163, 184,185,186,187);

  // Rock formations — bottom-left below lower path
  rocks(20, 63, 156,157,158,159, 180,181,182,183);
  rocks(30, 65, 160,161,162,163, 184,185,186,187);

  // Rock formations — top area
  rocks(35,  5, 160,161,162,163, 184,185,186,187);
  rocks(50,  8, 156,157,158,159, 180,181,182,183);

  // Stumps
  sm_tree(28, 14, 192,193,216,217);
  sm_tree(44,  7, 192,193,216,217);
  sm_tree(55, 64, 192,193,216,217);
  sm_tree(90, 72, 192,193,216,217);

  // Green bushes
  sm_tree(25, 10, 198,199,222,223);
  sm_tree(38,  4, 198,199,222,223);
  sm_tree(63, 67, 198,199,222,223);
  sm_tree(95, 50, 198,199,222,223);
  sm_tree(16, 40, 198,199,222,223);

  nature_map->LoadData(nature_data);
  nature_map->SetRenderLayer(1);
  render_system->Register(nature_map.get());

  // Solid tiles: big-tree trunks (mid+feet rows), small-tree bottoms, rocks, stumps, bushes
  for (int id : { 96,97,98,99, 100,101,102,103, 104,105,106,107,
                  108,109,110,111, 112,113,114,115 })
    nature_map->MarkSolidTile(id);

  for (int id : { 121,122, 125,126, 129,130, 133,134, 137,138 })
    nature_map->MarkSolidTile(id);

  for (int id : { 24,25, 26,27, 32,33, 34,35 })
    nature_map->MarkSolidTile(id);

  for (int id : { 156,157,158,159, 180,181,182,183,
                  160,161,162,163, 184,185,186,187 })
    nature_map->MarkSolidTile(id);

  for (int id : { 192,193,216,217, 198,199,222,223 })
    nature_map->MarkSolidTile(id);

  nature_map->SetTileCollisionLayer(2);
  nature_map->RegisterColliders(collision_system.get());

  auto static_behind = std::make_shared<StaticEntity>(
    Vector2{ 80.f, 80.f }, Vector2{ 60.f, 60.f }, ORANGE
  );
  static_behind->SetRenderLayer(-1);

  auto static_front = std::make_shared<StaticEntity>(
    Vector2{ 140.f, 80.f }, Vector2{ 60.f, 60.f }, SKYBLUE
  );
  static_front->SetRenderLayer(1);

  auto static_obstacle = std::make_shared<StaticEntity>(
    Vector2{ 300.f, 90.f }, Vector2{ 32.f, 32.f }, RED
  );
  auto obstacle_hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
    Rectangle{ 300.f, 90.f, 32.f, 32.f }
  );
  obstacle_hitbox->SetCollisionLayer(2);
  collision_system->Register(obstacle_hitbox.get());
  render_system->Register(obstacle_hitbox.get());
  static_obstacle->AddChild(obstacle_hitbox);

  p->SetCollisionSystem(collision_system.get());
  p->AddSolidMaskLayer(2);

  render_system->Register(static_behind.get());
  render_system->Register(static_obstacle.get());
  render_system->Register(static_front.get());

  render_system->Register(animation_player.get());
  render_system->Register(player_hitbox.get());

  // --- Camera (follows player) ---
  auto camera = std::make_shared<Shabby::Node::Camera2DNode>(render_system);

  // --- Ennemy spawn timer ---
  auto timer = std::make_shared<Shabby::Node::Timer>(0.1f);
  timer->_run_once = true;
  timer->timeout.connect([assets_registry = assets_registry.get(), ms, render_system, collision_system]() {
    for (int i = 0; i < ENNEMY_COUNT; ++i) {
      Vector2 spawn = ENNEMY_SPAWNS[i];

      auto e = std::make_shared<Ennemy>(spawn);

      auto es = std::make_shared<Shabby::Node::Sprite>(
        spawn,
        assets_registry->GetTexture(static_cast<int>(AssetId::BEAF))
      );

      auto ennemy_hitbox =
        std::make_shared<Shabby::Node::RectangleHitbox>((Rectangle{
          spawn.x, spawn.y, 16, 16
        }));

      ennemy_hitbox->SetCollisionLayer(3);
      render_system->Register(es.get());
      render_system->Register(ennemy_hitbox.get());
      collision_system->Register(ennemy_hitbox.get());

      e->AddChild(es);
      e->AddChild(ennemy_hitbox);

      ms->AddChildDeffered(e);
    }
  });

  // --- Build scene tree ---
  p->AddChild(animation_player);
  p->AddChild(player_hitbox);
  p->AddChild(camera);

  ms->AddChild(timer);
  ms->AddChild(tilemap);
  ms->AddChild(nature_map);
  ms->AddChild(p);
  ms->AddChild(static_behind);
  ms->AddChild(static_obstacle);
  ms->AddChild(static_front);

  // --- Run ---
  timer->Start();
  engine->SetAssetRegistry(std::move(assets_registry));
  engine->LoadTree(ms);
  engine->Run();

  return 0;
}
