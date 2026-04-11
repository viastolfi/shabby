#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "node/networking/network_node.h"
#include "node/sprite/sprite.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/render/render_system.h"
#include "core/drawable/idrawable.h"
#include "utils/raylog.h"

#include "player.h"
#include "enemy.h"

#include "raylib.h"

#include <unordered_map>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <memory>

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
  static constexpr float ENTITY_SIZE = 16.0f;

  GameClient(
      Shabby::Core::RenderSystem* render,
      Texture2D beaf,
      Texture2D monkey_idle,
      Texture2D monkey_walk)
    :_render(render), _beaf(beaf), _idle(monkey_idle), _walk(monkey_walk)
  {
    _hud = std::make_shared<HudDrawable>();
    _render->Register(_hud.get());
  }

  ~GameClient() = default;

  void Update(float dt) override
  {
    NetworkNode::Update(dt);
    SendInput();

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
  struct PlayerEntry {
    std::shared_ptr<Player> body;
    std::shared_ptr<Shabby::Node::AnimationPlayer> anim;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  struct EnemyEntry {
    std::shared_ptr<Enemy> body;
    std::shared_ptr<Shabby::Node::Sprite> sprite;
    std::shared_ptr<Shabby::Node::RectangleHitbox> hitbox;
  };

  Shabby::Core::RenderSystem* _render;
  Texture2D _beaf, _idle, _walk;
  int _local_id = -1;
  std::unordered_map<int, PlayerEntry> _players;
  std::unordered_map<int, EnemyEntry> _enemies;
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

  PlayerEntry& EnsurePlayer(int id, float x, float y)
  {
    auto it = _players.find(id);
    if (it != _players.end()) return it->second;

    auto body = std::make_shared<Player>(Vector2{x, y}, false);

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

    auto anim = std::make_shared<Shabby::Node::AnimationPlayer>();
    anim->Register("idle", idle);
    anim->Register("walk_down", walk_down);
    anim->Register("walk_up", walk_up);
    anim->Register("walk_left", walk_left);
    anim->Register("walk_right", walk_right);
    anim->Play("idle");

    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{x, y, ENTITY_SIZE, ENTITY_SIZE});

    _render->Register(anim.get());
    _render->Register(hitbox.get());

    body->AddChild(anim);
    body->AddChild(hitbox);
    AddChild(body);

    _players[id] = {body, anim, hitbox};
    return _players[id];
  }

  void RemovePlayer(int id)
  {
    auto it = _players.find(id);
    if (it == _players.end()) return;
    _render->Unregister(it->second.anim.get());
    _render->Unregister(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _players.erase(it);
  }

  EnemyEntry& EnsureEnemy(int id, float x, float y)
  {
    auto it = _enemies.find(id);
    if (it != _enemies.end()) return it->second;

    auto body = std::make_shared<Enemy>(Vector2{x, y}, false);
    auto sprite = std::make_shared<Shabby::Node::Sprite>(Vector2{x, y}, _beaf);
    auto hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
        Rectangle{x, y, ENTITY_SIZE, ENTITY_SIZE});

    _render->Register(sprite.get());
    _render->Register(hitbox.get());

    body->AddChild(sprite);
    body->AddChild(hitbox);
    AddChild(body);

    _enemies[id] = {body, sprite, hitbox};
    return _enemies[id];
  }

  void RemoveEnemy(int id)
  {
    auto it = _enemies.find(id);
    if (it == _enemies.end()) return;
    _render->Unregister(it->second.sprite.get());
    _render->Unregister(it->second.hitbox.get());
    RemoveChild(it->second.body);
    _enemies.erase(it);
  }

  void ParseState(const std::string& msg)
  {
    auto sep = msg.find('#');
    std::string players_part = (sep != std::string::npos) ? msg.substr(0, sep) : msg;
    std::string enemies_part = (sep != std::string::npos) ? msg.substr(sep + 1) : "";

    std::vector<int> active_players;
    if (!players_part.empty()) {
      std::istringstream ps(players_part);
      std::string entry;
      while (std::getline(ps, entry, ';')) {
        int id;
        float x, y, dx, dy;
        if (std::sscanf(entry.c_str(), "P%d,%f,%f,%f,%f", &id, &x, &y, &dx, &dy) == 5) {
          auto& pe = EnsurePlayer(id, x, y);
          pe.body->SetPos(Vector2{x, y});
          pe.body->SetInput(dx, dy);
          active_players.push_back(id);
        }
      }
    }

    std::vector<int> remove_p;
    for (auto& [id, pe] : _players) {
      bool found = false;
      for (int aid : active_players) if (aid == id) { found = true; break; }
      if (!found) remove_p.push_back(id);
    }
    for (int id : remove_p) RemovePlayer(id);

    std::vector<int> active_enemies;
    if (!enemies_part.empty()) {
      std::istringstream es(enemies_part);
      std::string entry;
      while (std::getline(es, entry, ';')) {
        int id;
        float x, y;
        if (std::sscanf(entry.c_str(), "E%d,%f,%f", &id, &x, &y) == 3) {
          auto& ee = EnsureEnemy(id, x, y);
          ee.body->SetPos(Vector2{x, y});
          active_enemies.push_back(id);
        }
      }
    }

    std::vector<int> remove_e;
    for (auto& [id, ee] : _enemies) {
      bool found = false;
      for (int aid : active_enemies) if (aid == id) { found = true; break; }
      if (!found) remove_e.push_back(id);
    }
    for (int id : remove_e) RemoveEnemy(id);
  }
};

#endif // GAME_CLIENT_H
