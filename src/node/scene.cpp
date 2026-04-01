#include "node/scene.h"

namespace Shabby::Node {

// TODO: not sure if this is safe
// keep an eye on that
Scene::Scene()
  :INode((Vector2){-1, -1})
{}

void Scene::Draw() 
{
  for (auto& c : _childs)
    c->Draw();
}

void Scene::Update(float dt)
{
  for (auto& c : _childs)
    c->Update(dt);

  for (auto& pa : _pending_add)
    _childs.push_back(pa);

  _pending_add.clear();
} 

}
