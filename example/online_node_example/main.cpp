#include <memory>
#include <string>
#include <cstring>

#include "utils/raylog.h"
#include "core/engine/engine.h"
#include "core/assets/assets_registry.h"
#include "node/networking/network_node.h"
#include "node/tilemap/tile_map_node.h"
#include "core/drawable/idrawable.h"

#include "server_scene.h"
#include "client_scene.h"

enum class AssetId {
  NINJA_IDLE = 0,
  NINJA_WALK,
  ENERGY_BALL,
  ONIGIRI,
  FIELD_TILESET,
  NATURE_TILESET
};

int main(int argc, char** argv)
{
  if (argc < 2) {
    Raylog::GetInstance(1).Log(1, "Usage: %s server|client [ip]", argv[0]);
    return 1;
  }

  Raylog::GetInstance(1).Log(1, "Logger Set");

  bool is_server = (std::strcmp(argv[1], "server") == 0);
  std::string ip = "127.0.0.1";
  if (!is_server && argc >= 3)
    ip = argv[2];

  if (is_server) {
    auto engine    = std::make_unique<Shabby::Core::Engine>(Shabby::Core::EngineMode::SERVER);
    auto collision = engine->GetCollisionSystem();

    auto net   = std::make_shared<Shabby::Node::NetworkNode>();
    auto scene = std::make_shared<ServerScene>(net, collision.get());
    scene->AddChild(net);

    net->CreateServer(7777);
    Raylog::GetInstance().Log(1, "Server started on port 7777");

    engine->LoadTree(scene);
    engine->Run();
  } else {
    auto engine = std::make_unique<Shabby::Core::Engine>(Shabby::Core::EngineMode::CLIENT);
    auto render  = engine->GetRenderSystem();

    auto assets = std::make_unique<Shabby::Core::AssetRegistry>();
    assets->LoadAll(
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::NINJA_IDLE),    "assets/actors/ninja_blue/Idle.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::NINJA_WALK),    "assets/actors/ninja_blue/Walk.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::ENERGY_BALL),   "assets/fx/EnergyBall.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::ONIGIRI),       "assets/items/Onigiri.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::FIELD_TILESET), "assets/tilesets/TilesetField.png"},
      Shabby::Core::AssetDesc{static_cast<int>(AssetId::NATURE_TILESET),"assets/tilesets/TilesetNature.png"}
    );

    auto net   = std::make_shared<Shabby::Node::NetworkNode>();
    auto scene = std::make_shared<ClientScene>(
        net,
        render,
        assets->GetTexture(static_cast<int>(AssetId::NINJA_IDLE)),
        assets->GetTexture(static_cast<int>(AssetId::NINJA_WALK)),
        assets->GetTexture(static_cast<int>(AssetId::ENERGY_BALL)),
        assets->GetTexture(static_cast<int>(AssetId::ONIGIRI)));
    scene->AddChild(net);

    // --- Background fill ---
    class BackgroundFill : public Shabby::Core::IDrawable {
    public:
      void Draw() override { DrawRectangle(-5000, -5000, 10000, 10000, Color{55, 95, 20, 255}); }
    };
    auto bg = std::make_shared<BackgroundFill>();
    bg->SetRenderLayer(-11);
    render->Register(bg.get());

    // --- Field tilemap ---
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

    for (int c = 0; c < MAP_COLS; ++c) {
      int wave = (c / 8) % 2;
      fill_rect(c, 20 + wave, 1, 3, SANDY_FILL);
      fill_rect(c, 58 + wave, 1, 3, SANDY_FILL);
    }
    for (int r = 0; r < MAP_ROWS; ++r) {
      int wave = (r / 7) % 2;
      fill_rect(60 + wave, r, 3, 1, SANDY_FILL);
    }

    fill_rect(10, 25, 18, 12, LIGHT_GRASS_FILL);
    fill_rect(75, 10, 20, 15, LIGHT_GRASS_FILL);
    fill_rect(40, 65, 15, 14, LIGHT_GRASS_FILL);
    fill_rect(90, 55, 22, 18, LIGHT_GRASS_FILL);
    fill_rect( 5, 68, 14, 10, LIGHT_GRASS_FILL);

    fill_rect(  0,  0, 8, 7,  DARK_GRASS_FILL);
    fill_rect(122,  0, 8, 8,  DARK_GRASS_FILL);
    fill_rect(  0, 82, 9, 8,  DARK_GRASS_FILL);
    fill_rect(121, 82, 9, 8,  DARK_GRASS_FILL);
    fill_rect( 32,  8, 6, 6,  DARK_GRASS_FILL);
    fill_rect( 95, 38, 7, 5,  DARK_GRASS_FILL);
    fill_rect( 15, 50, 5, 7,  DARK_GRASS_FILL);
    fill_rect( 72, 72, 8, 6,  DARK_GRASS_FILL);

    fill_rect_if( 14, 27, 4, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
    fill_rect_if( 80, 12, 5, 4, FLOWER_FILL, LIGHT_GRASS_FILL);
    fill_rect_if( 44, 68, 4, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
    fill_rect_if( 96, 60, 5, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
    fill_rect_if(  8, 70, 3, 3, FLOWER_FILL, LIGHT_GRASS_FILL);
    fill_rect_if( 20,  5, 3, 3, FLOWER_FILL, DARK_GRASS_FILL);
    fill_rect_if( 50, 40, 4, 3, FLOWER_FILL, DARK_GRASS_FILL);
    fill_rect_if(108, 28, 3, 4, FLOWER_FILL, DARK_GRASS_FILL);

    Shabby::Core::Tileset tileset{
      assets->GetTexture(static_cast<int>(AssetId::FIELD_TILESET)),
      TILE_W, TILE_H
    };
    auto tilemap = std::make_shared<Shabby::Node::TileMapNode>(
      Vector2{ -500.f, -432.f }, tileset, MAP_COLS, MAP_ROWS
    );
    tilemap->LoadData(tile_data);
    tilemap->SetRenderLayer(-10);
    render->Register(tilemap.get());
    scene->AddChild(tilemap);

    // --- Nature tilemap (scattered individual trees) ---
    Shabby::Core::Tileset nature_tileset{
      assets->GetTexture(static_cast<int>(AssetId::NATURE_TILESET)),
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

    big_tree(  3,  4, 72,73,74,75, 96,97,98,99, 121,122);
    sm_tree ( 22,  9, 0,1,24,25);
    sm_tree ( 50, 13, 10,11,34,35);
    big_tree( 82, 15, 76,77,78,79, 100,101,102,103, 125,126);
    sm_tree ( 15, 45, 2,3,26,27);
    sm_tree ( 75, 38, 8,9,32,33);
    big_tree( 35, 70, 88,89,90,91, 112,113,114,115, 137,138);
    sm_tree (100, 61, 0,1,24,25);
    sm_tree (112, 30, 10,11,34,35);
    sm_tree (  8, 76, 8,9,32,33);

    nature_map->LoadData(nature_data);
    nature_map->SetRenderLayer(1);
    render->Register(nature_map.get());
    scene->AddChild(nature_map);

    net->ConnectToServer(ip, 7777);
    Raylog::GetInstance().Log(1, "Connecting to %s:7777", ip.c_str());

    engine->SetAssetRegistry(std::move(assets));
    engine->LoadTree(scene);
    engine->Run();
  }

  return 0;
}

