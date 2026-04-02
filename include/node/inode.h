#ifndef INODE_HPP
#define INODE_HPP

#include <vector>
#include <memory>

#include "raylib.h"

namespace Shabby::Node {

class INode {
public:
  INode() = default;
  INode(Vector2 pos);
  virtual ~INode() = default;

  virtual void Update(float dt);

  const Vector2 GetPos() const;

  void AddChild(std::shared_ptr<INode> c);
  void AddChildDeffered(std::shared_ptr<INode> c);
protected:
  std::vector<std::shared_ptr<INode>> _childs;
  std::vector<std::shared_ptr<INode>> _pending_add;
  INode* _parent = nullptr;
  Vector2 _pos = {-1, -1};
};

} // namespace Shabby::Node

#endif // INODE_HPP
