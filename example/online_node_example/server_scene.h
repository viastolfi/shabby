#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include "node/inode.h"
#include "node/networking/network_node.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/collision_system.h"
#include "utils/raylog.h"

#include "server_player.h"
#include "server_enemy.h"
#include "state_protocol.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <string>

class ServerScene : public Shabby::Node::INode {
public:
  static constexpr float ENTITY_SIZE    = 16.0f;
  static constexpr int   MAX_ENEMIES    = 10;
  static constexpr float SPAWN_INTERVAL = 3.0f;
  static constexpr float BROADCAST_RATE = 1.0f / 60.0f;

  // net must be added as a child by the caller (main.cpp) after construction,
  // so that shared_from_this() is valid at that point.
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
  }

  void Update(float dt) override
  {
    ProcessRemovals();
    INode::Update(dt);
    SpawnTick(dt);
    BroadcastTick(dt);
  }

private:
  struct PlayerEntry {
    std::shared_ptr<ServerPlayer>                  body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  struct EnemyEntry {
    std::shared_ptr<ServerEnemy>                   body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  std::shared_ptr<Shabby::Node::NetworkNode>        _net;
  Shabby::Core::CollisionSystem*                    _collision;
  std::unordered_map<int, PlayerEntry>              _players;
  std::unordered_map<int, EnemyEntry>               _enemies;
  std::unordered_map<Shabby::Core::ICollider*, int> _hitbox_to_enemy;
  std::unordered_set<int>                           _players_to_remove;
  std::unordered_set<int>                           _enemies_to_remove;
  int   _next_enemy_id = 1;
  float _spawn_timer   = 0;
  float _bc_timer      = 0;

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

  void SpawnPlayer(int client_id)
  {
    float x = 100.0f + static_cast<float>(std::rand() % 600);
    float y = 100.0f + static_cast<float>(std::rand() % 400);

    auto body   = std::make_shared<ServerPlayer>(Vector2{x, y});
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{x, y, ENTITY_SIZE, ENTITY_SIZE});

    hitbox->entered.connect(
      [this](Shabby::Node::RectangleHitbox*, Shabby::Core::ICollider* from) {
        auto it = _hitbox_to_enemy.find(from);
        if (it != _hitbox_to_enemy.end())
          _enemies_to_remove.insert(it->second);
      });

    _collision->Register(hitbox.get());
    body->AddChild(hitbox);
    AddChildDeffered(body);

    _players[client_id] = {body, hitbox};
  }

  void DoRemovePlayer(int id)
  {
    auto it = _players.find(id);
    if (it == _players.end()) return;
    _collision->Unregister(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _players.erase(it);
  }

  void SpawnEnemy()
  {
    int id = _next_enemy_id++;
    float cx = 100.0f + static_cast<float>(std::rand() % 600);
    float cy = 100.0f + static_cast<float>(std::rand() % 400);

    auto body   = std::make_shared<ServerEnemy>(Vector2{cx, cy});
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{cx, cy, ENTITY_SIZE, ENTITY_SIZE});

    _collision->Register(hitbox.get());
    body->AddChild(hitbox);
    AddChild(body);

    _enemies[id] = {body, hitbox};
    _hitbox_to_enemy[hitbox.get()] = id;
    Raylog::GetInstance().Log(1, "Enemy %d spawned at (%.0f, %.0f)", id, cx, cy);
  }

  void DoRemoveEnemy(int id)
  {
    auto it = _enemies.find(id);
    if (it == _enemies.end()) return;
    _collision->Unregister(it->second.hitbox.get());
    _hitbox_to_enemy.erase(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _enemies.erase(it);
  }

  void ProcessRemovals()
  {
    for (int id : _players_to_remove)
      DoRemovePlayer(id);
    _players_to_remove.clear();

    for (int id : _enemies_to_remove)
      DoRemoveEnemy(id);
    _enemies_to_remove.clear();
  }

  void SpawnTick(float dt)
  {
    if (_players.empty()) return;
    _spawn_timer += dt;
    if (_spawn_timer >= SPAWN_INTERVAL &&
        static_cast<int>(_enemies.size()) < MAX_ENEMIES) {
      _spawn_timer = 0;
      SpawnEnemy();
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
          pe.body->GetInputDx(), pe.body->GetInputDy()});
    }

    std::vector<NetProtocol::EnemyState> enemy_states;
    enemy_states.reserve(_enemies.size());
    for (auto& [id, ee] : _enemies) {
      Vector2 pos = ee.body->GetPos();
      enemy_states.push_back({id, pos.x, pos.y});
    }

    _net->Broadcast("state", NetProtocol::EncodeState(player_states, enemy_states));
  }
};

#endif // SERVER_SCENE_H
