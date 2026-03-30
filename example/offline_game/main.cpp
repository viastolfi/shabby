#include <memory>
#include "actors/player_controller.h"
#include "actors/ennemy_controller.h"
#include "core/engine/engine.h"
#include "scene/scene.h"
#include "asset_ids.h"
#include "raylib.h"
#include "core/animation/animation.h"
#include "core/sprite/animated_sprite.h"
#include "core/animation/animation_player.h"

int main() 
{
  Raylog::GetInstance(1).Log(1, "Logger set");
  Shabby::EngineConfig config{
    800,
    450,
    "shabby",
    Shabby::STANDALONE,
  };  

  Shabby::Engine engine(config);
  auto& registry = engine.GetAssetRegistry();
  registry.LoadAll(
    Shabby::AssetDesc { 
      static_cast<int>(TextureId::MonkeyIdle), 
      "assets/actors/monkey/Idle.png",
      4, 1},
    Shabby::AssetDesc {
      static_cast<int>(TextureId::MonkeyWalk),
      "assets/actors/monkey/Walk.png",
      4, 4}
  );
  
  auto scene = std::make_unique<Shabby::Scene>(&registry);

  int idle_tid = static_cast<int>(TextureId::MonkeyIdle);
  std::shared_ptr<Shabby::AnimatedSprite> idle_sprite = 
    std::make_shared<Shabby::AnimatedSprite>(
        registry.GetTexture(idle_tid), 
        idle_tid,
        registry.GetTexturePath(idle_tid),
        registry.GetTextureCols(idle_tid),
        registry.GetTextureRows(idle_tid));

  int walk_tid = static_cast<int>(TextureId::MonkeyWalk);
  std::shared_ptr<Shabby::AnimatedSprite> walk_sprite = 
    std::make_shared<Shabby::AnimatedSprite>(
        registry.GetTexture(walk_tid), 
        walk_tid,
        registry.GetTexturePath(walk_tid),
        registry.GetTextureCols(walk_tid),
        registry.GetTextureRows(walk_tid));

  Shabby::Animation* idle_anim = new Shabby::Animation(
      idle_sprite, 0, 3, 0, 0, 3.0f);
  Shabby::Animation* walk_down  = new Shabby::Animation(
      walk_sprite, 0, 0, 0, 3, 5.0f);
  Shabby::Animation* walk_up    = new Shabby::Animation(
      walk_sprite, 1, 1, 0, 3, 5.0f);
  Shabby::Animation* walk_left  = new Shabby::Animation(
      walk_sprite, 2, 2, 0, 3, 5.0f);
  Shabby::Animation* walk_right = new Shabby::Animation(
      walk_sprite, 3, 3, 0, 3, 5.0f);

  std::unique_ptr<Shabby::AnimationPlayer> ap = 
    std::make_unique<Shabby::AnimationPlayer>();

  ap->RegisterAnimation(*idle_anim);
  ap->RegisterAnimation(*walk_down);
  ap->RegisterAnimation(*walk_up);
  ap->RegisterAnimation(*walk_left);
  ap->RegisterAnimation(*walk_right);
  ap->Play(0);

  std::unique_ptr<Shabby::AnimationPlayer> ape =
    std::make_unique<Shabby::AnimationPlayer>();

  ape->RegisterAnimation(*idle_anim);
  ape->RegisterAnimation(*walk_down);
  ape->RegisterAnimation(*walk_up);
  ape->RegisterAnimation(*walk_left);
  ape->RegisterAnimation(*walk_right);
  ape->Play(3);

  scene->AddEntity(
      std::make_unique<PlayerController>(),
      std::move(ap));

   auto ape_shared = std::make_shared<std::unique_ptr<Shabby::AnimationPlayer>>(
       std::move(ape));

   scene->ScheduleTaskAfter(
       1.f, [ape_shared](Shabby::Scene* s) {
          s->AddEntity(
            std::make_unique<EnnemyController>(),
            std::move(*ape_shared)); 
  });

  engine.LoadScene(std::move(scene));
  engine.Run();
}
