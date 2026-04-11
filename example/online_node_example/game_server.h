#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "node/networking/network_node.h"
#include "utils/raylog.h"

#include <unordered_map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <vector>

struct ServerPlayer {
  float x, y;
  float dx, dy;
};

struct ServerEnemy {
  float x, y;
  float cx, cy;
  float time;
};

class GameServer : public Shabby::Node::NetworkNode {
public:
  static constexpr float SPEED = 200.0f;
  static constexpr float SPAWN_INTERVAL = 3.0f;
  static constexpr int MAX_ENEMIES = 10;
  static constexpr float ENTITY_SIZE = 16.0f;
  static constexpr float BROADCAST_RATE = 1.0f / 60.0f;

  GameServer() = default;
  ~GameServer() = default;

  void Update(float dt) override
  {
    NetworkNode::Update(dt);
    UpdatePlayers(dt);
    UpdateEnemies(dt);
    CheckCollisions();
    SpawnTick(dt);

    _broadcast_timer += dt;
    if (_broadcast_timer >= BROADCAST_RATE) {
      _broadcast_timer = 0;
      BroadcastState();
    }
  }

protected:
  void _on_client_connected(int client_id) override
  {
    Raylog::GetInstance().Log(1, "Client %d connected", client_id);

    float x = 100.0f + static_cast<float>(std::rand() % 600);
    float y = 100.0f + static_cast<float>(std::rand() % 400);
    _players[client_id] = {x, y, 0, 0};

    GetServer().SendToClient(client_id, "welcome", std::to_string(client_id));
  }

  void _on_client_disconnected(int client_id) override
  {
    Raylog::GetInstance().Log(1, "Client %d disconnected", client_id);
    _players.erase(client_id);
  }

  void _on_receive_message(int client_id, const std::string& topic, const std::string& message) override
  {
    if (topic == "input") {
      float dx = 0, dy = 0;
      std::sscanf(message.c_str(), "%f,%f", &dx, &dy);

      auto it = _players.find(client_id);
      if (it != _players.end()) {
        it->second.dx = dx;
        it->second.dy = dy;
      }
    }
  }

private:
  std::unordered_map<int, ServerPlayer> _players;
  std::unordered_map<int, ServerEnemy> _enemies;
  int _next_enemy_id = 1;
  float _spawn_timer = SPAWN_INTERVAL;
  float _broadcast_timer = 0;

  void UpdatePlayers(float dt)
  {
    for (auto& [id, p] : _players) {
      p.x += p.dx * SPEED * dt;
      p.y += p.dy * SPEED * dt;
      if (p.x < 0) p.x = 0;
      if (p.y < 0) p.y = 0;
      if (p.x > 784) p.x = 784;
      if (p.y > 584) p.y = 584;
    }
  }

  void UpdateEnemies(float dt)
  {
    for (auto& [id, e] : _enemies) {
      e.time += dt;
      float radius = 100.0f;
      float tx = e.cx + radius * std::cos(e.time);
      float ty = e.cy + radius * std::sin(e.time);
      float dir_x = tx - e.x;
      float dir_y = ty - e.y;
      e.x += dir_x * dt;
      e.y += dir_y * dt;
    }
  }

  void CheckCollisions()
  {
    std::vector<int> to_remove;
    for (auto& [eid, e] : _enemies) {
      for (auto& [pid, p] : _players) {
        if (p.x < e.x + ENTITY_SIZE && p.x + ENTITY_SIZE > e.x &&
            p.y < e.y + ENTITY_SIZE && p.y + ENTITY_SIZE > e.y) {
          Raylog::GetInstance().Log(1, "Player %d hit enemy %d", pid, eid);
          to_remove.push_back(eid);
          break;
        }
      }
    }
    for (int eid : to_remove)
      _enemies.erase(eid);
  }

  void SpawnTick(float dt)
  {
    if (_players.empty())
      return;

    _spawn_timer += dt;
    if (_spawn_timer >= SPAWN_INTERVAL &&
        static_cast<int>(_enemies.size()) < MAX_ENEMIES) {
      _spawn_timer = 0;
      SpawnEnemy();
    }
  }

  void SpawnEnemy()
  {
    int id = _next_enemy_id++;
    float cx = 100.0f + static_cast<float>(std::rand() % 600);
    float cy = 100.0f + static_cast<float>(std::rand() % 400);
    _enemies[id] = {cx, cy, cx, cy, 0};
    Raylog::GetInstance().Log(1, "Enemy %d spawned at (%.0f, %.0f)", id, cx, cy);
  }

  void BroadcastState()
  {
    std::ostringstream oss;
    bool first = true;
    for (auto& [id, p] : _players) {
      if (!first) oss << ';';
      char buf[128];
      std::snprintf(buf, sizeof(buf), "P%d,%.1f,%.1f,%.0f,%.0f",
          id, p.x, p.y, p.dx, p.dy);
      oss << buf;
      first = false;
    }
    oss << '#';
    first = true;
    for (auto& [id, e] : _enemies) {
      if (!first) oss << ';';
      char buf[128];
      std::snprintf(buf, sizeof(buf), "E%d,%.1f,%.1f", id, e.x, e.y);
      oss << buf;
      first = false;
    }

    Broadcast("state", oss.str());
  }
};

#endif // GAME_SERVER_H
