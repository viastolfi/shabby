#ifndef CLIENT_SCENE_H
#define CLIENT_SCENE_H

#include "node/inode.h"
#include "node/networking/network_node.h"
#include "node/camera/camera.h"
#include "node/sprite/sprite.h"
#include "node/sprite/animation_player.h"
#include "core/render/render_system.h"
#include "core/drawable/idrawable.h"
#include "utils/raylog.h"

#include "remote_player.h"
#include "remote_enemy.h"
#include "state_protocol.h"

#include "raylib.h"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <memory>

class HudDrawable : public Shabby::Core::IDrawable {
public:
  int player_count = 0;
  int enemy_count  = 0;
  int local_id     = -1;

  void Draw() override
  {
    char info[64];
    std::snprintf(info, sizeof(info), "Players: %d  Enemies: %d  (P%d)",
        player_count, enemy_count, local_id);
    DrawText(info, 10, 10, 20, DARKGRAY);
  }
};

class ClientScene : public Shabby::Node::INode {
public:
  static constexpr float ENTITY_SIZE = 16.0f;

  // net must be added as a child by the caller (main.cpp) after construction.
  ClientScene(
      std::shared_ptr<Shabby::Node::NetworkNode>  net,
      std::weak_ptr<Shabby::Core::RenderSystem>   render,
      Texture2D beaf,
      Texture2D idle,
      Texture2D walk)
    : _net(net), _render(render), _beaf(beaf), _idle(idle), _walk(walk)
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
    _hud->enemy_count  = static_cast<int>(_enemies.size());
    _hud->local_id     = _local_id;
  }

private:
  struct PlayerEntry {
    std::shared_ptr<RemotePlayer>                      body;
    std::shared_ptr<Shabby::Node::AnimationPlayer>     anim;
  };

  struct EnemyEntry {
    std::shared_ptr<RemoteEnemy>                   body;
    std::shared_ptr<Shabby::Node::Sprite>          sprite;
  };

  std::shared_ptr<Shabby::Node::NetworkNode>     _net;
  std::weak_ptr<Shabby::Core::RenderSystem>      _render;
  Texture2D                                      _beaf, _idle, _walk;
  int                                            _local_id = -1;
  std::unordered_map<int, PlayerEntry>           _players;
  std::unordered_map<int, EnemyEntry>            _enemies;
  std::unordered_set<int>                        _players_to_remove;
  std::unordered_set<int>                        _enemies_to_remove;
  std::shared_ptr<HudDrawable>                   _hud;

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
      AttachCameraToLocalPlayer();
    } else if (topic == "state") {
      ParseState(msg);
    }
  }

  void AttachCameraToLocalPlayer()
  {
    auto it = _players.find(_local_id);
    if (it == _players.end() || _camera) return;
    _camera = std::make_shared<Shabby::Node::Camera2DNode>(_render);
    it->second.body->AddChild(_camera);
  }

  PlayerEntry& EnsurePlayer(int id, float x, float y)
  {
    auto it = _players.find(id);
    if (it != _players.end()) return it->second;

    auto body = std::make_shared<RemotePlayer>(Vector2{x, y}, _idle, _walk);
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
  }

  EnemyEntry& EnsureEnemy(int id, float x, float y)
  {
    auto it = _enemies.find(id);
    if (it != _enemies.end()) return it->second;

    auto body   = std::make_shared<RemoteEnemy>(Vector2{x, y}, _beaf);
    auto sprite = body->GetSprite();

    if (auto r = _render.lock())
      r->Register(sprite.get());

    body->AddChild(sprite);
    AddChildDeffered(body);
    _enemies[id] = {body, sprite};
    return _enemies[id];
  }

  void DoRemoveEnemy(int id)
  {
    auto it = _enemies.find(id);
    if (it == _enemies.end()) return;
    if (auto r = _render.lock())
      r->Unregister(it->second.sprite.get());
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

  void ParseState(const std::string& msg)
  {
    std::vector<NetProtocol::PlayerState> player_states;
    std::vector<NetProtocol::EnemyState>  enemy_states;
    NetProtocol::DecodeState(msg, player_states, enemy_states);

    std::vector<int> active_players;
    for (auto& p : player_states) {
      auto& pe = EnsurePlayer(p.id, p.x, p.y);
      pe.body->SetPos(Vector2{p.x, p.y});
      pe.body->SetAnimation(p.dx, p.dy);
      active_players.push_back(p.id);
    }

    for (auto& [id, pe] : _players) {
      bool found = false;
      for (int aid : active_players) if (aid == id) { found = true; break; }
      if (!found) _players_to_remove.insert(id);
    }

    std::vector<int> active_enemies;
    for (auto& e : enemy_states) {
      auto& ee = EnsureEnemy(e.id, e.x, e.y);
      ee.body->SetPos(Vector2{e.x, e.y});
      active_enemies.push_back(e.id);
    }

    for (auto& [id, ee] : _enemies) {
      bool found = false;
      for (int aid : active_enemies) if (aid == id) { found = true; break; }
      if (!found) _enemies_to_remove.insert(id);
    }
  }

  std::shared_ptr<Shabby::Node::Camera2DNode> _camera;
};

#endif // CLIENT_SCENE_H
