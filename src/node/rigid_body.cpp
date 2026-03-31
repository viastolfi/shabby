#include "node/rigid_body.h"

namespace Shabby::Node {

RigidBody::RigidBody(Vector2 pos)
  :INode(pos)
{}

void RigidBody::Update(float dt) 
{
  _physics_process(dt);
  _pos = Vector2Add(_pos, Vector2Scale(_dir, dt));

  for (auto& c : _childs)
    c->Update(dt);
}

void RigidBody::Draw() 
{
  for (auto& c : _childs)
    c->Draw();
}

} // namespace Shabby::Node
