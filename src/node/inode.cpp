#include "node/inode.h"

namespace Shabby::Node {

INode::INode(Vector2 pos)
  :_pos(pos)
{}

void INode::AddChild(std::shared_ptr<INode> c) 
{
  c->_parent = this;
  _childs.push_back(c);
}

void INode::AddChildDeffered(std::shared_ptr<INode> c)
{
  c->_parent = this;
  _pending_add.push_back(c);
}

void INode::Update(float dt) 
{
  for (auto& c : _childs)
    c->Update(dt);

  for (auto& pa : _pending_add)
    _childs.push_back(pa);

  _pending_add.clear();
}

const Vector2 INode::GetPos() const
{
  return _pos;
}

} // namespace Shabby::Node
