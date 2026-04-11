#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "node/networking/network_node.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/collision_system.h"
#include "utils/raylog.h"

#include "player.h"
#include "enemy.h"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <memory>

class GameServer : public Shabby::Node::NetworkNode {
public:
  static constexpr float ENTITY_SIZE = 16.0f;
  static constexpr int MAX_ENEMIES = 10;
  static constexpr float SPAWN_INTERVAL = 3.0f;
  static constexpr float BROADCAST_RATE = 1.0f / 60.0f;

  GameServer(Shabby::Core::CollisionSystem* cs)
    :_collision(cs)
  {}

  ~GameServer() = default;

  void Update(float dt) override
  {
    ProcessRemovals();
    NetworkNode::Update(dt);
    SpawnTick(dt);
    BroadcastTick(dt);
  }

protected:
  void _on_client_connected(int client_id) override
  {
    Raylog::GetInstance().Log(1, "Client %d connected", client_id);
    SpawnPlayer(client_id);
    GetServer().SendToClient(client_id, "welcome", std::to_string(client_id));
  }

  void _on_client_disconnected(int client_id) override
  {
    Raylog::GetInstance().Log(1, "Client %d disconnected", client_id);
    RemovePlayer(client_id);
  }

  void _on_receive_message(int client_id, const std::string& topic, const std::string& message) override
  {
    if (topic == "input") {
      float dx = 0, dy = 0;
      std::sscanf(message.c_str(), "%f,%f", &dx, &dy);
      auto it = _players.find(client_id);
      if (it != _players.end())
        it->second.body->SetInput(dx, dy);
    }
  }

private:
  struct PlayerEntry {
    std::shared_ptr<Player> body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  struct EnemyEntry {
    std::shared_ptr<Enemy> body;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  Shabby::Core::CollisionSystem* _collision;
  std::unordered_map<int, PlayerEntry> _players;
  std::unordered_map<int, EnemyEntry> _enemies;
  std::unordered_map<Shabby::Core::ICollider*, int> _hitbox_to_enemy;
  std::unordered_set<int> _enemies_to_remove;
  int _next_enemy_id = 1;
  float _spawn_timer = 0;
  float _bc_timer = 0;

  void SpawnPlayer(int client_id)
  {
    float x = 100.0f + static_cast<float>(std::rand() % 600);
    float y = 100.0f + static_cast<float>(std::rand() % 400);

    auto body = std::make_shared<Player>(Vector2{x, y}, true);
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
    AddChild(body);

    _players[client_id] = {body, hitbox};
  }

  void RemovePlayer(int id)
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

    auto body = std::make_shared<Enemy>(Vector2{cx, cy}, true);
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{cx, cy, ENTITY_SIZE, ENTITY_SIZE});

    _collision->Register(hitbox.get());
    body->AddChild(hitbox);
    AddChildDeffered(body);

    _enemies[id] = {body, hitbox};
    _hitbox_to_enemy[hitbox.get()] = id;
    Raylog::GetInstance().Log(1, "Enemy %d spawned at (%.0f, %.0f)", id, cx, cy);
  }

  void RemoveEnemy(int id)
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
    for (int id : _enemies_to_remove)
      RemoveEnemy(id);
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

    std::ostringstream oss;
    bool first = true;
    for (auto& [id, pe] : _players) {
      if (!first) oss << ';';
      Vector2 pos = pe.body->GetPos();
      char buf[128];
      std::snprintf(buf, sizeof(buf), "P%d,%.1f,%.1f,%.0f,%.0f",
          id, pos.x, pos.y, pe.body->GetInputDx(), pe.body->GetInputDy());
      oss << buf;
      first = false;
    }
    oss << '#';
    first = true;
    for (auto& [id, ee] : _enemies) {
      if (!first) oss << ';';
      Vector2 pos = ee.body->GetPos();
      char buf[128];
      std::snprintf(buf, sizeof(buf), "E%d,%.1f,%.1f", id, pos.x, pos.y);
      oss << buf;
      first = false;
    }
    Broadcast("state", oss.str());
  }
};

#endif // GAME_SERVER_H
