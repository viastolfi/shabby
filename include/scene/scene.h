#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "entities/entity.h"
#include "entities/entity_manager/entity_manager.h"
#include "replication/snapshot/snapshot.h"
#include "replication/snapshot/world_snapshot.h"
#include "networking/client.h"
#include "core/factories/sprite_factory.h"
#include "core/assets/assets_registry.h"
#include "entities/controllers/entity_controller.h"
#include "replication/snapshot/entity_snapshot.h"
#include "core/factories/sprite_factory.h"
#include "core/factories/entity_factory.h"
#include "core/scheduler/scheduler.h"

namespace engine {

class Scene {
public:
  explicit Scene(AssetRegistry* assets_registry);
  explicit Scene();
  virtual ~Scene();

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;

  void Update(float dt);
  void Draw() const;
  void ApplySnapshot(Snapshot& s);
  void ApplyWorldSnapshot(WorldSnapshot& snapshot);

  Entity* AddEntity(
      std::unique_ptr<IEntityController> controller,
      int texture_id);

  Entity* AddEntity(
    std::unique_ptr<IEntityController> controller,
    int texture_id,
    float x, float y, float width, float height);
  
  void AddEntity(
      uint64_t id,
      int texture_id,
      Vector2 position);
  
  void RemoveEntity(uint64_t id);
  
  void ScheduleTaskAfter(float seconds, std::function<void(Scene*)> task);
  void ScheduleTaskEvery(float seconds, std::function<void(Scene*)> task);
  
  Scheduler* GetScheduler() { return _task_scheduler.get(); }
  const std::vector<Entity*>& GetEntities() const;
  
  WorldSnapshot CreateWorldSnapshot() const;
  
private:
  std::unique_ptr<Scheduler> _task_scheduler;
  std::unique_ptr<EntityManager> _entity_manager;
};

} // namespace engine

#endif // SCENE_H
