#include <memory>

#include "core/engine/engine.h"
#include "node/sprite/sprite.h"
#include "node/sprite/animated_sprite.h"
#include "node/sprite/animation_player.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "node/camera/camera.h"
#include "node/timer.h"
#include "utils/raylog.h"

#include "main_scene.h"
#include "background.h"
#include "player.h"
#include "ennemy.h"

enum class AssetId {
  BEAF = 0,
  MONKEY_IDLE,
  MONKEY_WALK
};

// Positions spread across the world, outside the initial 800x600 view
// (player starts at {100,100}, initial viewport covers roughly {-300,-200} to {500,400})
static const Vector2 ENNEMY_SPAWNS[] = {
  {  800,  100 },   // far right
  {  100,  700 },   // far below
  { -400,  200 },   // far left
  {  500, -300 },   // far above
  {  900,  700 },   // bottom-right corner
};
static constexpr int ENNEMY_COUNT = 5;

int main(void)
{
  Raylog::GetInstance(1).Log(1, "Logger Set");

  // --- Engine setup ---
  auto engine        = std::make_unique<Shabby::Core::Engine>();
  auto render_system = engine->GetRenderSystem();
  auto collision_system = engine->GetCollisionSystem();

  auto assets_registry = std::make_unique<Shabby::Core::AssetRegistry>();
  assets_registry->LoadAll(
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::BEAF),        "assets/Beaf.png" },
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::MONKEY_IDLE), "assets/actors/monkey/Idle.png" },
    Shabby::Core::AssetDesc{ static_cast<int>(AssetId::MONKEY_WALK), "assets/actors/monkey/Walk.png" }
  );

  // --- Scene & player ---
  auto ms = std::make_shared<MainScene>();
  auto p  = std::make_shared<Player>((Vector2){ 100, 100 });

  auto player_hitbox = std::make_shared<Shabby::Node::RectangleHitbox>(
    (Rectangle){ 100, 100, 16, 16 }
  );

  player_hitbox->entered.connect(
    [p](
      Shabby::Node::RectangleHitbox* hit, 
      Shabby::Core::ICollider* from) 
    {
      p->_on_hitbox_entered(hit, from);
    }
  );

  player_hitbox->exited.connect(
    [p](
      Shabby::Node::RectangleHitbox* hit,
      Shabby::Core::ICollider* from)
    {
      p->_on_hitbox_exited(hit, from); 
    }
  );
  collision_system->Register(player_hitbox.get());

  // --- Animations ---
  auto idle       = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_IDLE)),
    0, 3, 0, 0, 4, 1, 3.0f
  );
  auto walk_down  = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    0, 0, 0, 3, 4, 4, 3.0f
  );
  auto walk_up    = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    1, 1, 0, 3, 4, 4, 3.0f
  );
  auto walk_left  = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    2, 2, 0, 3, 4, 4, 3.0f
  );
  auto walk_right = std::make_shared<Shabby::Node::AnimatedSprite>(
    (Vector2){ 100, 100 },
    assets_registry->GetTexture(static_cast<int>(AssetId::MONKEY_WALK)),
    3, 3, 0, 3, 4, 4, 3.0f
  );

  auto animation_player = std::make_shared<Shabby::Node::AnimationPlayer>();
  animation_player->Register("idle",       idle);
  animation_player->Register("walk_up",    walk_up);
  animation_player->Register("walk_down",  walk_down);
  animation_player->Register("walk_left",  walk_left);
  animation_player->Register("walk_right", walk_right);
  animation_player->Play("idle");

  // --- Background (must be registered first → drawn behind everything) ---
  auto bg = std::make_shared<Background>();
  render_system->Register(bg.get());

  render_system->Register(animation_player.get());
  render_system->Register(player_hitbox.get());

  // --- Camera (follows player) ---
  auto camera = std::make_shared<Shabby::Node::Camera2DNode>(render_system);

  // --- Ennemy spawn timer ---
  auto timer = std::make_shared<Shabby::Node::Timer>(0.1f);
  timer->_run_once = true;
  timer->timeout.connect([assets_registry = assets_registry.get(), ms, render_system, collision_system]() {
    for (int i = 0; i < ENNEMY_COUNT; ++i) {
      Vector2 spawn = ENNEMY_SPAWNS[i];

      auto e = std::make_shared<Ennemy>(spawn);

      auto es = std::make_shared<Shabby::Node::Sprite>(
        spawn,
        assets_registry->GetTexture(static_cast<int>(AssetId::BEAF))
      );

      auto ennemy_hitbox =
        std::make_shared<Shabby::Node::RectangleHitbox>((Rectangle{
          spawn.x, spawn.y, 16, 16
        }));

      render_system->Register(es.get());
      render_system->Register(ennemy_hitbox.get());
      collision_system->Register(ennemy_hitbox.get());

      e->AddChild(es);
      e->AddChild(ennemy_hitbox);

      ms->AddChildDeffered(e);
    }
  });

  // --- Build scene tree ---
  p->AddChild(animation_player);
  p->AddChild(player_hitbox);
  p->AddChild(camera);

  ms->AddChild(timer);
  ms->AddChild(p);

  // --- Run ---
  timer->Start();
  engine->SetAssetRegistry(std::move(assets_registry));
  engine->LoadTree(ms);
  engine->Run();

  return 0;
}
