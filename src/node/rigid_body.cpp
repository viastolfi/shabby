#include "node/rigid_body.h"

namespace Shabby::Node {

RigidBody::RigidBody(Vector2 pos)
  :INode(pos)
{}

void RigidBody::Update(float dt) 
{
  // TODO: check if we really need dt
  (void)dt;
  _pos += _dir;
}

void RigidBody::Draw() 
{
  for (auto c : _childs)
    c->Draw();
}

} // namespace Shabby::Node
