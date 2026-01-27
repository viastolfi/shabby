#include "scene.h"

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
  if (_entity_manager) 
    _entity_manager->AddEntity(std::move(entity));
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

} // namespace engine


