#ifndef CLIENT_SCENE_H
#define CLIENT_SCENE_H

#include "node/inode.h"
#include "node/networking/network_node.h"
#include "node/camera/camera.h"
#include "node/sprite/animation_player.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/sprite.h"
#include "core/render/render_system.h"
#include "core/drawable/idrawable.h"
#include "utils/raylog.h"

#include "remote_player.h"
#include "remote_projectile.h"
#include "remote_food.h"
#include "state_protocol.h"

#include "raylib.h"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>

class HudDrawable : public Shabby::Core::IDrawable {
public:
  int player_count = 0;
  int local_id     = -1;
  int local_hp     = 100;
  int local_score  = 0;

  void Draw() override
  {
    char info[128];
    std::snprintf(info, sizeof(info), "HP: %d  Score: %d  Players: %d  (P%d)",
        local_hp, local_score, player_count, local_id);
    DrawText(info, 10, 10, 20, DARKGRAY);
  }
};

class ClientScene : public Shabby::Node::INode {
public:
  static constexpr float ENTITY_SIZE = 16.0f;

  ClientScene(
      std::shared_ptr<Shabby::Node::NetworkNode>  net,
      std::weak_ptr<Shabby::Core::RenderSystem>   render,
      Texture2D ninja_idle,
      Texture2D ninja_walk,
      Texture2D energy_ball,
      Texture2D onigiri)
    : _net(net), _render(render)
    , _ninja_idle(ninja_idle), _ninja_walk(ninja_walk)
    , _energy_ball(energy_ball), _onigiri(onigiri)
  {
    _hud = std::make_shared<HudDrawable>();
    if (auto r = render.lock())
      r->RegisterHUD(_hud.get());

    _net->connected.connect([this]() {
      Raylog::GetInstance().Log(1, "Connected to server");
    });
    _net->disconnected.connect([this]() {
      Raylog::GetInstance().Log(1, "Disconnected from server");
    });
    _net->client_message.connect([this](const std::string& topic, const std::string& msg) {
      _on_message(topic, msg);
    });
  }

  ~ClientScene()
  {
    if (auto r = _render.lock())
      r->UnregisterHUD(_hud.get());
  }

  void Update(float dt) override
  {
    ProcessRemovals();
    INode::Update(dt);
    SendInput();
    _hud->player_count = static_cast<int>(_players.size());
    _hud->local_id     = _local_id;
  }

private:
  struct PlayerEntry {
    std::shared_ptr<RemotePlayer>                  body;
    std::shared_ptr<Shabby::Node::AnimationPlayer> anim;
  };

  struct ProjEntry {
    std::shared_ptr<RemoteProjectile>              body;
    std::shared_ptr<Shabby::Node::AnimatedSprite>  sprite;
  };

  struct FoodEntry {
    std::shared_ptr<RemoteFood>           body;
    std::shared_ptr<Shabby::Node::Sprite> sprite;
  };

  std::shared_ptr<Shabby::Node::NetworkNode>     _net;
  std::weak_ptr<Shabby::Core::RenderSystem>      _render;
  Texture2D                                      _ninja_idle, _ninja_walk;
  Texture2D                                      _energy_ball, _onigiri;
  int                                            _local_id = -1;
  std::unordered_map<int, PlayerEntry>           _players;
  std::unordered_map<int, ProjEntry>             _projectiles;
  std::unordered_map<int, FoodEntry>             _foods;
  std::unordered_map<int, Vector2>               _last_player_pos;
  std::unordered_set<int>                        _players_to_remove;
  std::unordered_set<int>                        _projs_to_remove;
  std::unordered_set<int>                        _food_to_remove;
  std::shared_ptr<HudDrawable>                   _hud;
  std::shared_ptr<Shabby::Node::Camera2DNode>    _camera;

  void SendInput()
  {
    float dx = 0, dy = 0;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx -= 1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx += 1;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    dy -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  dy += 1;

    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.0f,%.0f", dx, dy);
    _net->Send("input", buf);
  }

  void _on_message(const std::string& topic, const std::string& msg)
  {
    if (topic == "welcome") {
      _local_id = std::atoi(msg.c_str());
      Raylog::GetInstance().Log(1, "Assigned ID: %d", _local_id);
    } else if (topic == "state") {
      ParseState(msg);
    }
  }

  PlayerEntry& EnsurePlayer(int id, float x, float y)
  {
    auto it = _players.find(id);
    if (it != _players.end()) return it->second;

    auto body = std::make_shared<RemotePlayer>(Vector2{x, y}, _ninja_idle, _ninja_walk);
    auto anim = body->GetAnimPlayer();

    if (auto r = _render.lock())
      r->Register(anim.get());

    body->AddChild(anim);

    if (id == _local_id && !_camera) {
      _camera = std::make_shared<Shabby::Node::Camera2DNode>(_render);
      body->AddChild(_camera);
    }

    AddChildDeffered(body);
    _players[id] = {body, anim};
    return _players[id];
  }

  void DoRemovePlayer(int id)
  {
    auto it = _players.find(id);
    if (it == _players.end()) return;
    if (auto r = _render.lock())
      r->Unregister(it->second.anim.get());
    if (id == _local_id) _camera.reset();
    RemoveChild(it->second.body);
    _players.erase(it);
    _last_player_pos.erase(id);
  }

  ProjEntry& EnsureProjectile(int id, float x, float y)
  {
    auto it = _projectiles.find(id);
    if (it != _projectiles.end()) return it->second;

    auto body   = std::make_shared<RemoteProjectile>(Vector2{x, y}, _energy_ball);
    auto sprite = body->GetSprite();

    if (auto r = _render.lock())
      r->Register(sprite.get());

    body->AddChild(sprite);
    AddChildDeffered(body);
    _projectiles[id] = {body, sprite};
    return _projectiles[id];
  }

  void DoRemoveProjectile(int id)
  {
    auto it = _projectiles.find(id);
    if (it == _projectiles.end()) return;
    if (auto r = _render.lock())
      r->Unregister(it->second.sprite.get());
    RemoveChild(it->second.body);
    _projectiles.erase(it);
  }

  FoodEntry& EnsureFood(int id, float x, float y)
  {
    auto it = _foods.find(id);
    if (it != _foods.end()) return it->second;

    auto body   = std::make_shared<RemoteFood>(Vector2{x, y}, _onigiri);
    auto sprite = body->GetSprite();

    if (auto r = _render.lock())
      r->Register(sprite.get());

    body->AddChild(sprite);
    AddChildDeffered(body);
    _foods[id] = {body, sprite};
    return _foods[id];
  }

  void DoRemoveFood(int id)
  {
    auto it = _foods.find(id);
    if (it == _foods.end()) return;
    if (auto r = _render.lock())
      r->Unregister(it->second.sprite.get());
    RemoveChild(it->second.body);
    _foods.erase(it);
  }

  void ProcessRemovals()
  {
    for (int id : _players_to_remove) DoRemovePlayer(id);
    _players_to_remove.clear();

    for (int id : _projs_to_remove) DoRemoveProjectile(id);
    _projs_to_remove.clear();

    for (int id : _food_to_remove) DoRemoveFood(id);
    _food_to_remove.clear();
  }

  void ParseState(const std::string& msg)
  {
    std::vector<NetProtocol::PlayerState>     player_states;
    std::vector<NetProtocol::ProjectileState> proj_states;
    std::vector<NetProtocol::FoodState>       food_states;
    NetProtocol::DecodeState(msg, player_states, proj_states, food_states);

    std::vector<int> active_players;
    for (auto& p : player_states) {
      auto& pe = EnsurePlayer(p.id, p.x, p.y);
      Vector2 new_pos = {p.x, p.y};
      Vector2 old_pos = _last_player_pos.count(p.id) ? _last_player_pos[p.id] : new_pos;
      pe.body->SetPos(new_pos);
      pe.body->SetAnimation(new_pos.x - old_pos.x, new_pos.y - old_pos.y);
      _last_player_pos[p.id] = new_pos;
      if (p.id == _local_id) {
        _hud->local_hp    = p.hp;
        _hud->local_score = p.score;
      }
      active_players.push_back(p.id);
    }
    for (auto& [id, _] : _players) {
      bool found = false;
      for (int aid : active_players) if (aid == id) { found = true; break; }
      if (!found) _players_to_remove.insert(id);
    }

    std::vector<int> active_projs;
    for (auto& r : proj_states) {
      auto& pe = EnsureProjectile(r.id, r.x, r.y);
      pe.body->SetPos(Vector2{r.x, r.y});
      active_projs.push_back(r.id);
    }
    for (auto& [id, _] : _projectiles) {
      bool found = false;
      for (int aid : active_projs) if (aid == id) { found = true; break; }
      if (!found) _projs_to_remove.insert(id);
    }

    std::vector<int> active_foods;
    for (auto& f : food_states) {
      auto& fe = EnsureFood(f.id, f.x, f.y);
      fe.body->SetPos(Vector2{f.x, f.y});
      active_foods.push_back(f.id);
    }
    for (auto& [id, _] : _foods) {
      bool found = false;
      for (int aid : active_foods) if (aid == id) { found = true; break; }
      if (!found) _food_to_remove.insert(id);
    }
  }
};

#endif // CLIENT_SCENE_H
