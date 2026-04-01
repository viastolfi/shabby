#ifndef INODE_HPP
#define INODE_HPP

#include <vector>
#include <memory>

#include "raylib.h"

namespace Shabby::Node {

class INode {
public:
  INode(Vector2 pos);
  virtual ~INode() = default;

  virtual void Update(float dt);
  virtual void Draw() = 0;

  void AddChild(std::shared_ptr<INode> c);
  void AddChildDeffered(std::shared_ptr<INode> c);

  const Vector2 GetPos() const;
protected:
  std::vector<std::shared_ptr<INode>> _childs;
  std::vector<std::shared_ptr<INode>> _pending_add;
  Vector2 _pos;
  INode* _parent = nullptr;
};

} // namespace Shabby::Node

#endif // INODE_HPP
