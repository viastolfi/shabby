#include "node/inode.h"

namespace Shabby::Node {

INode::INode(Vector2 pos)
  :_pos(pos)
{}

void INode::AddChild(std::shared_ptr<INode> c) 
{
  c->_parent = shared_from_this();
  _childs.push_back(c);
}

void INode::AddChildDeffered(std::shared_ptr<INode> c)
{
  c->_parent = shared_from_this();
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

void INode::SetPos(Vector2 pos)
{
  _pos = pos;
}

void INode::RemoveChild(std::shared_ptr<INode> c)
{
  _childs.erase(
    std::remove(_childs.begin(), _childs.end(), c),
    _childs.end());
}

} // namespace Shabby::Node
