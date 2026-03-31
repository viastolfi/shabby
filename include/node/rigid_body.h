#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "node/inode.h"
#include "utils/vector2.h"

namespace Shabby::Node {

class RigidBody : public INode {
public:
  RigidBody(Vector2 pos);
  ~RigidBody() = default;

  void Update(float dt) override;
  void Draw() override;
protected:
  Vector2 _dir;
};
  
} // namespace Shabby::Node

#endif // RIGID_BODY_H
