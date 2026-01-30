#include "scene/scene.h"
#include "replication/snapshot/entity_snapshot.h"

namespace engine {

Scene::Scene()
  :_entity_manager(std::make_unique<EntityManager>())
{}

Scene::~Scene()
{
  _entity_manager.reset();
}

void Scene::AddEntity(std::unique_ptr<Entity> entity) 
{
  if (_entity_manager) {
    entity->LoadSprite();
    _entity_manager->AddEntity(std::move(entity));
  }
}

void Scene::UpdateScene(float dt)
{
  if (_entity_manager)
    _entity_manager->UpdateAll(dt);
}

void Scene::DrawScene() const
{
  if (_entity_manager)
    _entity_manager->DrawAll();
}

void Scene::ApplySnapshot(Snapshot& s) 
{
  EntitySnapshot* es = dynamic_cast<EntitySnapshot*>(&s);
  if (es != nullptr) {
    _entity_manager->UpdateOne(es->GetId(), es->GetDirection());
  }
}

Packet Scene::GenerateWorldSnapshot() const
{
  Packet snapshot(PacketType::WORLD_SNAPSHOT);
  
  if (_entity_manager) {
    const auto& entities = _entity_manager->GetEntities();

    size_t count = entities.size();
    snapshot.Write(count);
    
    for (const auto& entity : entities) {
      snapshot.Write(entity->GetId());
      snapshot.Write(entity->GetPos());
    }
  }
  
  return snapshot;
}

void Scene::ApplyWorldSnapshot(Packet& snapshot, size_t ignore_entity_id)
{
  if (!_entity_manager) return;
  
  size_t count;
  snapshot.Read(count);

  for (size_t i = 0; i < count; ++i) {
    size_t id;
    Vector2 pos;
    snapshot.Read(id);
    snapshot.Read(pos);
    
    if (id == ignore_entity_id) {
      continue;
    }
    
    bool entity_exists = false;
    for (const auto& e : _entity_manager->GetEntities()) {
      if (e->GetId() == id) {
        entity_exists = true;
        break;
      }
    }
    
    if (entity_exists) {
      _entity_manager->UpdatePosition(id, pos);
    } else {
      AddEntity<Entity>(
          std::make_unique<AnimatedSprite>("assets/actors/monkey/Idle.png", 4, 1, 3),
          pos, id);
    }
  }
}

void Scene::ApplyNewEntitySnapshot(
    Packet& snapshot, 
    size_t ignore_entity_id,
    std::function<std::unique_ptr<Sprite>(int)> sprite_factory)
{
  if (!_entity_manager) return;

  size_t entity_id;
  int texture_id;
  Vector2 pos;

  snapshot.Read(entity_id);
  snapshot.Read(texture_id);
  snapshot.Read(pos);

  if (entity_id == ignore_entity_id) return;

  auto sprite = sprite_factory(texture_id);
  if (sprite) {
    AddEntity<Entity>(std::move(sprite), pos, entity_id);
  }
}

} // namespace engine
