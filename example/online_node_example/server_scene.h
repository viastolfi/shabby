#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include "node/inode.h"
#include "node/networking/network_node.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/collision_system.h"
#include "utils/raylog.h"

#include "server_player.h"
#include "server_projectile.h"
#include "state_protocol.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>

class ServerScene : public Shabby::Node::INode {
public:
  static constexpr float BROADCAST_RATE     = 1.0f / 60.0f;
  static constexpr float SHOOT_INTERVAL     = 1.5f;
  static constexpr float PROJ_TTL           = 2.0f;
  static constexpr float PROJ_SPEED         = 250.0f;
  static constexpr float FOOD_SPAWN_INTERVAL = 4.0f;
  static constexpr int   MAX_FOOD           = 8;
  static constexpr int   PLAYER_SIZE        = 16;
  static constexpr int   FOOD_SIZE          = 16;
  static constexpr int   START_HP           = 100;
  static constexpr int   PROJ_DAMAGE        = 20;
  static constexpr int   FOOD_HEAL          = 10;
  static constexpr float WORLD_MIN_X        = -480.0f;
  static constexpr float WORLD_MAX_X        = 1560.0f;
  static constexpr float WORLD_MIN_Y        = -412.0f;
  static constexpr float WORLD_MAX_Y        =  988.0f;

  ServerScene(
      std::shared_ptr<Shabby::Node::NetworkNode> net,
      Shabby::Core::CollisionSystem* cs)
    : _net(net), _collision(cs)
  {
    _net->client_connected.connect([this](int id) {
      _on_client_connected(id);
    });
    _net->client_disconnected.connect([this](int id) {
      _players_to_remove.insert(id);
    });
    _net->server_message.connect([this](int cid, const std::string& topic, const std::string& msg) {
      _on_message(cid, topic, msg);
    });
    InitTrees();
  }

  void Update(float dt) override
  {
    ProcessRemovals();
    INode::Update(dt);
    ShootTick(dt);
    ProjectileTick(dt);
    FoodSpawnTick(dt);
    BroadcastTick(dt);
  }

private:
  struct PlayerEntry {
    std::shared_ptr<ServerPlayer>                  body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
    int   hp          = START_HP;
    int   score       = 0;
    float shoot_timer = 0;
  };

  struct ProjectileEntry {
    std::shared_ptr<ServerProjectile>              body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
    int   owner_id;
    float ttl;
  };

  struct FoodEntry {
    std::shared_ptr<Shabby::Node::INode>           body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  std::shared_ptr<Shabby::Node::NetworkNode>        _net;
  Shabby::Core::CollisionSystem*                    _collision;
  std::unordered_map<int, PlayerEntry>              _players;
  std::unordered_map<int, ProjectileEntry>          _projectiles;
  std::unordered_map<int, FoodEntry>                _foods;
  std::unordered_map<Shabby::Core::ICollider*, int> _hitbox_to_player_id;
  std::unordered_map<Shabby::Core::ICollider*, int> _projhitbox_to_id;
  std::unordered_map<Shabby::Core::ICollider*, int> _foodhitbox_to_id;
  std::unordered_set<int>                           _players_to_remove;
  std::unordered_set<int>                           _projs_to_remove;
  std::unordered_set<int>                           _food_to_remove;
  std::unordered_set<int>                           _players_died_this_frame;
  std::vector<std::pair<int,int>>                   _respawn_queue;
  int   _next_proj_id      = 1;
  int   _next_food_id      = 1;
  float _bc_timer          = 0;
  float _food_spawn_timer  = 0;
  std::vector<std::shared_ptr<Shabby::Node::RectangleHitbox>> _tree_hitboxes;

  void InitTrees()
  {
    static constexpr float TILE_PX = 16.0f;
    static constexpr float MAP_OX  = -500.0f;
    static constexpr float MAP_OY  = -432.0f;

    auto add_tree = [&](float cx, float cy, float w, float h) {
      auto hb = std::make_shared<Shabby::Node::RectangleHitbox>(
          Rectangle{cx, cy, w, h});
      hb->SetCollisionLayer(4);
      _collision->Register(hb.get());
      _tree_hitboxes.push_back(hb);
    };
    auto big = [&](int c, int r) {
      add_tree(MAP_OX + c * TILE_PX, MAP_OY + r * TILE_PX, 4 * TILE_PX, 3 * TILE_PX);
    };
    auto sm = [&](int c, int r) {
      add_tree(MAP_OX + c * TILE_PX, MAP_OY + r * TILE_PX, 2 * TILE_PX, 2 * TILE_PX);
    };

    big(  3,  4);
    sm ( 22,  9);
    sm ( 50, 13);
    big( 82, 15);
    sm ( 15, 45);
    sm ( 75, 38);
    big( 35, 70);
    sm (100, 61);
    sm (112, 30);
    sm (  8, 76);
  }

  void _on_client_connected(int client_id)
  {
    Raylog::GetInstance().Log(1, "Client %d connected", client_id);
    SpawnPlayer(client_id);
    _net->SendToClient(client_id, "welcome", std::to_string(client_id));
  }

  void _on_message(int client_id, const std::string& topic, const std::string& msg)
  {
    if (topic != "input") return;
    float dx = 0, dy = 0;
    std::sscanf(msg.c_str(), "%f,%f", &dx, &dy);
    auto it = _players.find(client_id);
    if (it != _players.end())
      it->second.body->SetInput(dx, dy);
  }

  Vector2 RandomSpawnPos()
  {
    int rx = std::rand() % static_cast<int>(WORLD_MAX_X - WORLD_MIN_X - 100);
    int ry = std::rand() % static_cast<int>(WORLD_MAX_Y - WORLD_MIN_Y - 100);
    return { WORLD_MIN_X + 50.0f + static_cast<float>(rx),
             WORLD_MIN_Y + 50.0f + static_cast<float>(ry) };
  }

  void SpawnPlayer(int client_id)
  {
    Vector2 pos = RandomSpawnPos();
    auto body   = std::make_shared<ServerPlayer>(pos);
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{pos.x, pos.y, (float)PLAYER_SIZE, (float)PLAYER_SIZE});

    hitbox->SetCollisionLayer(1);
    hitbox->AddMaskLayer(2);
    hitbox->AddMaskLayer(3);

    hitbox->entered.connect(
      [this, client_id](Shabby::Node::RectangleHitbox*, Shabby::Core::ICollider* from) {
        _on_player_hitbox_entered(client_id, from);
      });

    body->SetCollisionSystem(_collision);
    body->AddSolidMaskLayer(4);
    _collision->Register(hitbox.get());
    body->AddChild(hitbox);
    AddChildDeffered(body);

    _hitbox_to_player_id[hitbox.get()] = client_id;
    _players[client_id] = {body, hitbox, START_HP, 0, 0.0f};
  }

  void _on_player_hitbox_entered(int client_id, Shabby::Core::ICollider* from)
  {
    auto proj_it = _projhitbox_to_id.find(from);
    if (proj_it != _projhitbox_to_id.end()) {
      int proj_id = proj_it->second;
      auto& proj  = _projectiles[proj_id];
      if (proj.owner_id == client_id) return;
      if (_projs_to_remove.count(proj_id)) return;
      if (_players_died_this_frame.count(client_id)) return;

      _projs_to_remove.insert(proj_id);

      auto player_it = _players.find(client_id);
      if (player_it == _players.end()) return;
      player_it->second.hp -= PROJ_DAMAGE;
      if (player_it->second.hp <= 0) {
        _players_died_this_frame.insert(client_id);
        _respawn_queue.push_back({client_id, proj.owner_id});
      }
      return;
    }

    auto food_it = _foodhitbox_to_id.find(from);
    if (food_it != _foodhitbox_to_id.end()) {
      int food_id = food_it->second;
      if (_food_to_remove.count(food_id)) return;
      _food_to_remove.insert(food_id);

      auto player_it = _players.find(client_id);
      if (player_it == _players.end()) return;
      player_it->second.hp = std::min(START_HP, player_it->second.hp + FOOD_HEAL);
    }
  }

  void DoRemovePlayer(int id)
  {
    auto it = _players.find(id);
    if (it == _players.end()) return;
    _collision->Unregister(it->second.hitbox.get());
    _hitbox_to_player_id.erase(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _players.erase(it);

    std::vector<int> owned;
    for (auto& [pid, pe] : _projectiles)
      if (pe.owner_id == id) owned.push_back(pid);
    for (int pid : owned)
      DoRemoveProjectile(pid);
  }

  void SpawnProjectile(int owner_id, Vector2 pos, Vector2 dir)
  {
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len == 0) return;
    dir.x /= len; dir.y /= len;

    int id  = _next_proj_id++;
    Vector2 vel = {dir.x * PROJ_SPEED, dir.y * PROJ_SPEED};

    auto body   = std::make_shared<ServerProjectile>(pos, vel);
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{pos.x, pos.y, ServerProjectile::SIZE, ServerProjectile::SIZE});

    hitbox->SetCollisionLayer(2);

    _collision->Register(hitbox.get());
    body->AddChild(hitbox);
    AddChild(body);

    _projhitbox_to_id[hitbox.get()] = id;
    _projectiles[id] = {body, hitbox, owner_id, PROJ_TTL};
  }

  void DoRemoveProjectile(int id)
  {
    auto it = _projectiles.find(id);
    if (it == _projectiles.end()) return;
    _collision->Unregister(it->second.hitbox.get());
    _projhitbox_to_id.erase(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _projectiles.erase(it);
  }

  void SpawnFood()
  {
    int id      = _next_food_id++;
    Vector2 pos = RandomSpawnPos();
    auto body   = std::make_shared<Shabby::Node::INode>(pos);
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{pos.x, pos.y, (float)FOOD_SIZE, (float)FOOD_SIZE});

    hitbox->SetCollisionLayer(3);
    _collision->Register(hitbox.get());
    body->AddChild(hitbox);
    AddChild(body);

    _foodhitbox_to_id[hitbox.get()] = id;
    _foods[id] = {body, hitbox};
  }

  void DoRemoveFood(int id)
  {
    auto it = _foods.find(id);
    if (it == _foods.end()) return;
    _collision->Unregister(it->second.hitbox.get());
    _foodhitbox_to_id.erase(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _foods.erase(it);
  }

  void ProcessRemovals()
  {
    for (auto& [dead_id, killer_id] : _respawn_queue) {
      auto pit = _players.find(dead_id);
      if (pit == _players.end()) continue;
      pit->second.hp = START_HP;
      pit->second.body->SetPos(RandomSpawnPos());
      auto kit = _players.find(killer_id);
      if (kit != _players.end()) {
        kit->second.score++;
        Raylog::GetInstance().Log(1, "Player %d killed player %d (score %d)",
            killer_id, dead_id, kit->second.score);
      }
    }
    _respawn_queue.clear();
    _players_died_this_frame.clear();

    for (int id : _players_to_remove) DoRemovePlayer(id);
    _players_to_remove.clear();

    for (int id : _projs_to_remove) DoRemoveProjectile(id);
    _projs_to_remove.clear();

    for (int id : _food_to_remove) DoRemoveFood(id);
    _food_to_remove.clear();
  }

  void ShootTick(float dt)
  {
    for (auto& [id, pe] : _players) {
      pe.shoot_timer += dt;
      if (pe.shoot_timer >= SHOOT_INTERVAL) {
        pe.shoot_timer -= SHOOT_INTERVAL;
        SpawnProjectile(id, pe.body->GetPos(),
            {pe.body->GetFaceDx(), pe.body->GetFaceDy()});
      }
    }
  }

  void ProjectileTick(float dt)
  {
    for (auto& [id, pe] : _projectiles) {
      pe.ttl -= dt;
      if (pe.ttl <= 0)
        _projs_to_remove.insert(id);
    }
  }

  void FoodSpawnTick(float dt)
  {
    _food_spawn_timer += dt;
    if (_food_spawn_timer >= FOOD_SPAWN_INTERVAL &&
        static_cast<int>(_foods.size()) < MAX_FOOD) {
      _food_spawn_timer -= FOOD_SPAWN_INTERVAL;
      SpawnFood();
    }
  }

  void BroadcastTick(float dt)
  {
    _bc_timer += dt;
    if (_bc_timer < BROADCAST_RATE) return;
    _bc_timer = 0;

    std::vector<NetProtocol::PlayerState> player_states;
    player_states.reserve(_players.size());
    for (auto& [id, pe] : _players) {
      Vector2 pos = pe.body->GetPos();
      player_states.push_back({id, pos.x, pos.y,
          pe.body->GetInputDx(), pe.body->GetInputDy(), pe.hp, pe.score});
    }

    std::vector<NetProtocol::ProjectileState> proj_states;
    proj_states.reserve(_projectiles.size());
    for (auto& [id, pe] : _projectiles) {
      Vector2 pos = pe.body->GetPos();
      proj_states.push_back({id, pos.x, pos.y});
    }

    std::vector<NetProtocol::FoodState> food_states;
    food_states.reserve(_foods.size());
    for (auto& [id, fe] : _foods) {
      Vector2 pos = fe.body->GetPos();
      food_states.push_back({id, pos.x, pos.y});
    }

    _net->Broadcast("state", NetProtocol::EncodeState(player_states, proj_states, food_states));
  }
};

#endif // SERVER_SCENE_H
