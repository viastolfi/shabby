#include "core/engine/engine.h"
#include "core/assets/assets_registry.h"
#include "networking/server.h"
#include "server/logic/example_logic.h"
#include <memory>
#include <cmath>
#include "actors/player_controller.h"
#include "core/animation/animation.h"
#include "core/sprite/animated_sprite.h"
#include "core/animation/animation_player.h"

enum class TextureId
{
  MonkeyIdle,
  MonkeyWalk,
  Monk
};

class RemoteAnimController : public engine::IEntityController {
public:
  ~RemoteAnimController() override = default;
  void OnInit(engine::Entity* entity) override { 
    _prev_pos = entity->_pos;
  }
  void OnUpdate(engine::Entity* entity, float dt) override {
    (void)dt;
    float dx = entity->_pos.x - _prev_pos.x;
    float dy = entity->_pos.y - _prev_pos.y;

    if (dx != 0.f || dy != 0.f) {
      _idle_frames = 0;
      if (fabsf(dy) >= fabsf(dx))
        entity->PlayAnimation(dy > 0 ? 1 : 2);
      else
        entity->PlayAnimation(dx < 0 ? 3 : 4);
    } else {
      if (++_idle_frames > 5)
        entity->PlayAnimation(0);
    }
    _prev_pos = entity->_pos;
  }
private:
  Vector2 _prev_pos = {0.f, 0.f};
  int _idle_frames = 100;
};

void RunServerEngine()
{
  engine::ServerConf conf{8080, 2};
  std::unique_ptr<engine::ServerLogic> logic = 
    std::make_unique<ExampleLogic>();
  engine::Engine engine(conf, std::move(logic));
  engine.Run();
}

void RunClientEngine() 
{
  engine::EngineConfig config{
    800,
    450,
    "shabby",
    engine::CLIENT,
  };  

  engine::Engine engine(config);
  auto& registry = engine.GetAssetRegistry();
  registry.LoadAll(
    engine::AssetDesc { 
      static_cast<int>(TextureId::MonkeyIdle), 
      "assets/actors/monkey/Idle.png",
      4, 1},
    engine::AssetDesc {
      static_cast<int>(TextureId::MonkeyWalk),
      "assets/actors/monkey/Walk.png",
      4, 4},
    engine::AssetDesc { 
      static_cast<int>(TextureId::Monk), 
      "assets/actors/monk/Idle.png",
      6, 1}
  );
  
  auto scene = std::make_unique<engine::Scene>(&registry);

  int idle_tid = static_cast<int>(TextureId::MonkeyIdle);
  auto idle_sprite = std::make_shared<engine::AnimatedSprite>(
      registry.GetTexture(idle_tid), idle_tid,
      registry.GetTexturePath(idle_tid),
      registry.GetTextureCols(idle_tid),
      registry.GetTextureRows(idle_tid));

  int walk_tid = static_cast<int>(TextureId::MonkeyWalk);
  auto walk_sprite = std::make_shared<engine::AnimatedSprite>(
      registry.GetTexture(walk_tid), walk_tid,
      registry.GetTexturePath(walk_tid),
      registry.GetTextureCols(walk_tid),
      registry.GetTextureRows(walk_tid));

  auto* idle  = new engine::Animation(idle_sprite, 0, 3, 0, 0, 3.0f);
  auto* down  = new engine::Animation(walk_sprite, 0, 0, 0, 3, 5.0f);
  auto* up    = new engine::Animation(walk_sprite, 1, 1, 0, 3, 5.0f);
  auto* left  = new engine::Animation(walk_sprite, 2, 2, 0, 3, 5.0f);
  auto* right = new engine::Animation(walk_sprite, 3, 3, 0, 3, 5.0f);

  auto ap = std::make_unique<engine::AnimationPlayer>();
  ap->RegisterAnimation(*idle);
  ap->RegisterAnimation(*down);
  ap->RegisterAnimation(*up);
  ap->RegisterAnimation(*left);
  ap->RegisterAnimation(*right);
  ap->Play(0);

  auto controller = std::make_unique<PlayerController>(engine.GetClient());
  auto* local_entity = scene->AddEntity(
      std::move(controller), 
      static_cast<int>(TextureId::MonkeyIdle));
  local_entity->_is_local = true;
  local_entity->SetAnimationPlayer(std::move(ap));
  std::cout << "[Main] Created local entity with temp id=" << local_entity->_id << std::endl;

  scene->SetRemoteEntitySetup(
      [idle_sprite, walk_sprite](engine::Entity* e) {
    auto* r_idle  = new engine::Animation(idle_sprite, 0, 3, 0, 0, 3.0f);
    auto* r_down  = new engine::Animation(walk_sprite, 0, 0, 0, 3, 5.0f);
    auto* r_up    = new engine::Animation(walk_sprite, 1, 1, 0, 3, 5.0f);
    auto* r_left  = new engine::Animation(walk_sprite, 2, 2, 0, 3, 5.0f);
    auto* r_right = new engine::Animation(walk_sprite, 3, 3, 0, 3, 5.0f);

    auto rap = std::make_unique<engine::AnimationPlayer>();
    rap->RegisterAnimation(*r_idle);
    rap->RegisterAnimation(*r_down);
    rap->RegisterAnimation(*r_up);
    rap->RegisterAnimation(*r_left);
    rap->RegisterAnimation(*r_right);
    rap->Play(0);

    e->SetAnimationPlayer(std::move(rap));
    e->SetController(std::make_unique<RemoteAnimController>());
  });

  engine.LoadScene(std::move(scene));
  engine.Run();
}

int main(int argc, char* argv[]) 
{
  auto has_arg = [argc, argv](const char* lo) -> bool 
  {
    for (int i = 0; i < argc; i++)
     if (strcmp(argv[i], lo) == 0)
      return true; 

    return false;
  };

  if (has_arg("client")) {
    RunClientEngine();
  }
  else if (has_arg("server")) {
    RunServerEngine();
  }
  
  return 0;
}
