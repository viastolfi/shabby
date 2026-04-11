#ifndef INODE_HPP
#define INODE_HPP

#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>

#include "raylib.h"

namespace Shabby::Node {

class INode : public std::enable_shared_from_this<INode> {
public:
  INode() = default;
  INode(Vector2 pos);
  virtual ~INode() = default;

  virtual void Update(float dt);

  const Vector2 GetPos() const;
  void SetPos(Vector2 pos);

  template<typename T>
  std::vector<std::shared_ptr<T>> GetChildOfType()
  {
    //TODO: enhance error message
    static_assert(std::is_base_of<INode, T>::value,
        "Must be a INode"); 
    std::vector<std::shared_ptr<T>> childs;

    for(const auto& c : _childs) {
      if (auto casted = std::dynamic_pointer_cast<T>(c))
          childs.push_back(casted);

    }

    return childs;
  }

  void AddChild(std::shared_ptr<INode> c);
  void AddChildDeffered(std::shared_ptr<INode> c);
  void RemoveChild(std::shared_ptr<INode> c);
protected:
  std::vector<std::shared_ptr<INode>> _childs;
  std::vector<std::shared_ptr<INode>> _pending_add;
  std::weak_ptr<INode> _parent; 
  Vector2 _pos = {-1, -1};
};

} // namespace Shabby::Node

#endif // INODE_HPP
