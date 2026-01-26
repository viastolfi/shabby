#ifndef SCENE_H
#define SCENE_H

#include "engine/entities/entities.h"
#include "engine/entities/entity_manager.h"

namespace engine {

class Scene {
public:
  explicit Scene();
  virtual ~Scene();

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;

  void AddEntity(std::unique_ptr<Entity> e);

  void UpdateScene(float dt);
  void DrawScene() const;
private:
  std::unique_ptr<EntityManager> _entity_manager;
};

} // namespace engine

#endif // SCENE_H
