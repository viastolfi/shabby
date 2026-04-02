#include "node/rigid_body.h"

namespace Shabby::Node {

RigidBody::RigidBody(Vector2 pos)
  :INode(pos)
{}

void RigidBody::Update(float dt) 
{
  _physics_process(dt);
  _pos = Vector2Add(_pos, Vector2Scale(_dir, dt));

  INode::Update(dt);
}

} // namespace Shabby::Node
