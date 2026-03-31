#include "node/inode.h"

namespace Shabby::Node {

INode::INode(Vector2 pos)
  :_pos(pos)
{}

void INode::AddChild(std::shared_ptr<INode> c) {
  _childs.push_back(c);
}

void INode::Update(float dt) 
{
  for (auto c : _childs)
    c->Update(dt);
}

} // namespace Shabby::Node
