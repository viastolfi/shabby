#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "node/networking/network_node.h"
#include "node/collision_shape/remote_body.h"
#include "node/sprite/sprite.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/render/render_system.h"
#include "core/physics/collision_system.h"
#include "core/drawable/idrawable.h"
#include "utils/raylog.h"

#include "raylib.h"

#include <unordered_map>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <memory>

struct PlayerNodes {
  std::shared_ptr<Shabby::Node::RemoteBody> body;
  std::shared_ptr<Shabby::Node::AnimationPlayer> anim_player;
  std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  float dx = 0, dy = 0;
};

struct EnemyNodes {
  std::shared_ptr<Shabby::Node::RemoteBody> body;
  std::shared_ptr<Shabby::Node::Sprite> sprite;
  std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
};

class HudDrawable : public Shabby::Core::IDrawable {
public:
  int player_count = 0;
  int enemy_count = 0;
  int local_id = -1;

  void Draw() override
  {
    char info[64];
    std::snprintf(info, sizeof(info), "Players: %d  Enemies: %d  (P%d)",
        player_count, enemy_count, local_id);
    DrawText(info, 10, 10, 20, DARKGRAY);
  }
};

class GameClient : public Shabby::Node::NetworkNode {
public:
  GameClient(
      Shabby::Core::RenderSystem* render_system,
      Shabby::Core::CollisionSystem* collision_system,
      Texture2D beaf,
      Texture2D monkey_idle,
      Texture2D monkey_walk)
    :_render_system(render_system),
     _collision_system(collision_system),
     _beaf(beaf), _idle(monkey_idle), _walk(monkey_walk)
  {
    _hud = std::make_shared<HudDrawable>();
    _render_system->Register(_hud.get());
  }

  ~GameClient() = default;

  void Update(float dt) override
  {
    NetworkNode::Update(dt);
    SendInput();
    UpdateAnimations();

    _hud->player_count = static_cast<int>(_players.size());
    _hud->enemy_count = static_cast<int>(_enemies.size());
    _hud->local_id = _local_id;
  }

protected:
  void _on_connected() override
  {
    Raylog::GetInstance().Log(1, "Connected to server");
  }

  void _on_disconnected() override
  {
    Raylog::GetInstance().Log(1, "Disconnected from server");
  }

  void _on_receive_message(const std::string& topic, const std::string& message) override
  {
    if (topic == "welcome") {
      _local_id = std::atoi(message.c_str());
      Raylog::GetInstance().Log(1, "Assigned ID: %d", _local_id);
    }
    else if (topic == "state") {
      ParseState(message);
    }
  }

private:
  Shabby::Core::RenderSystem* _render_system;
  Shabby::Core::CollisionSystem* _collision_system;
  Texture2D _beaf, _idle, _walk;

  int _local_id = -1;
  std::unordered_map<int, PlayerNodes> _players;
  std::unordered_map<int, EnemyNodes> _enemies;
  std::shared_ptr<HudDrawable> _hud;

  void SendInput()
  {
    float dx = 0, dy = 0;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx -= 1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx += 1;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    dy -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  dy += 1;

    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.0f,%.0f", dx, dy);
    Send("input", buf);
  }

  PlayerNodes& EnsurePlayer(int id, float x, float y)
  {
    auto it = _players.find(id);
    if (it != _players.end())
      return it->second;

    PlayerNodes pn;
    pn.body = std::make_shared<Shabby::Node::RemoteBody>(Vector2{x, y});

    auto idle = std::make_shared<Shabby::Node::AnimatedSprite>(
        Vector2{x, y}, _idle, 0, 3, 0, 0, 4, 1, 3.0f);
    auto walk_down = std::make_shared<Shabby::Node::AnimatedSprite>(
        Vector2{x, y}, _walk, 0, 0, 0, 3, 4, 4, 3.0f);
    auto walk_up = std::make_shared<Shabby::Node::AnimatedSprite>(
        Vector2{x, y}, _walk, 1, 1, 0, 3, 4, 4, 3.0f);
    auto walk_left = std::make_shared<Shabby::Node::AnimatedSprite>(
        Vector2{x, y}, _walk, 2, 2, 0, 3, 4, 4, 3.0f);
    auto walk_right = std::make_shared<Shabby::Node::AnimatedSprite>(
        Vector2{x, y}, _walk, 3, 3, 0, 3, 4, 4, 3.0f);

    pn.anim_player = std::make_shared<Shabby::Node::AnimationPlayer>();
    pn.anim_player->Register("idle", idle);
    pn.anim_player->Register("walk_down", walk_down);
    pn.anim_player->Register("walk_up", walk_up);
    pn.anim_player->Register("walk_left", walk_left);
    pn.anim_player->Register("walk_right", walk_right);
    pn.anim_player->Play("idle");

    pn.hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{x, y, 16, 16});

    _render_system->Register(pn.anim_player.get());
    _render_system->Register(pn.hitbox.get());

    if (_collision_system)
      _collision_system->Register(pn.hitbox.get());

    pn.body->AddChild(pn.anim_player);
    pn.body->AddChild(pn.hitbox);
    AddChild(pn.body);

    _players[id] = pn;
    return _players[id];
  }

  void RemovePlayer(int id)
  {
    auto it = _players.find(id);
    if (it == _players.end()) return;

    _render_system->Unregister(it->second.anim_player.get());
    _render_system->Unregister(it->second.hitbox.get());
    _collision_system->Unregister(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _players.erase(it);
  }

  EnemyNodes& EnsureEnemy(int id, float x, float y)
  {
    auto it = _enemies.find(id);
    if (it != _enemies.end())
      return it->second;

    EnemyNodes en;
    en.body = std::make_shared<Shabby::Node::RemoteBody>(Vector2{x, y});
    en.sprite = std::make_shared<Shabby::Node::Sprite>(Vector2{x, y}, _beaf);
    en.hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{x, y, 16, 16});

    _render_system->Register(en.sprite.get());
    _render_system->Register(en.hitbox.get());

    en.body->AddChild(en.sprite);
    en.body->AddChild(en.hitbox);
    AddChild(en.body);

    _enemies[id] = en;
    return _enemies[id];
  }

  void RemoveEnemy(int id)
  {
    auto it = _enemies.find(id);
    if (it == _enemies.end()) return;

    _render_system->Unregister(it->second.sprite.get());
    _render_system->Unregister(it->second.hitbox.get());
    _render_system->Unregister(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _enemies.erase(it);
  }

  void UpdateAnimations()
  {
    for (auto& [id, pn] : _players) {
      if (pn.dx == 0 && pn.dy == 0)
        pn.anim_player->Play("idle");
      else if (pn.dy > 0)
        pn.anim_player->Play("walk_down");
      else if (pn.dy < 0)
        pn.anim_player->Play("walk_up");
      else if (pn.dx < 0)
        pn.anim_player->Play("walk_left");
      else
        pn.anim_player->Play("walk_right");
    }
  }

  void ParseState(const std::string& msg)
  {
    auto sep = msg.find('#');
    std::string players_part = (sep != std::string::npos) ? msg.substr(0, sep) : msg;
    std::string enemies_part = (sep != std::string::npos) ? msg.substr(sep + 1) : "";

    std::vector<int> active_player_ids;
    if (!players_part.empty()) {
      std::istringstream pstream(players_part);
      std::string entry;
      while (std::getline(pstream, entry, ';')) {
        int id;
        float x, y, dx, dy;
        if (std::sscanf(entry.c_str(), "P%d,%f,%f,%f,%f", &id, &x, &y, &dx, &dy) == 5) {
          auto& pn = EnsurePlayer(id, x, y);
          pn.body->SetPos(Vector2{x, y});
          pn.dx = dx;
          pn.dy = dy;
          active_player_ids.push_back(id);
        }
      }
    }

    std::vector<int> to_remove_p;
    for (auto& [id, pn] : _players) {
      bool found = false;
      for (int aid : active_player_ids)
        if (aid == id) { found = true; break; }
      if (!found) to_remove_p.push_back(id);
    }
    for (int id : to_remove_p)
      RemovePlayer(id);

    std::vector<int> active_enemy_ids;
    if (!enemies_part.empty()) {
      std::istringstream estream(enemies_part);
      std::string entry;
      while (std::getline(estream, entry, ';')) {
        int id;
        float x, y;
        if (std::sscanf(entry.c_str(), "E%d,%f,%f", &id, &x, &y) == 3) {
          auto& en = EnsureEnemy(id, x, y);
          en.body->SetPos(Vector2{x, y});
          active_enemy_ids.push_back(id);
        }
      }
    }

    std::vector<int> to_remove_e;
    for (auto& [id, en] : _enemies) {
      bool found = false;
      for (int aid : active_enemy_ids)
        if (aid == id) { found = true; break; }
      if (!found) to_remove_e.push_back(id);
    }
    for (int id : to_remove_e)
      RemoveEnemy(id);
  }
};

#endif // GAME_CLIENT_H
