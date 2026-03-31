#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "node/inode.h"
#include "utils/vector2.h"

#include "raymath.h"

namespace Shabby::Node {

class RigidBody : public INode {
public:
  RigidBody(Vector2 pos);
  ~RigidBody() = default;

  void Update(float dt) override final;
  void Draw() override final;

  virtual void _physics_process(float dt) = 0;
protected:
  Vector2 _dir;
};
  
} // namespace Shabby::Node

#endif // RIGID_BODY_H
