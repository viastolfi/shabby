#include "node/collision_shape/remote_body.h"

namespace Shabby::Node {

RemoteBody::RemoteBody(Vector2 pos)
  :INode(pos)
{}

void RemoteBody::Update(float dt)
{
  INode::Update(dt);
}

} // namespace Shabby::Node
