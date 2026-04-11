#ifndef REMOTE_BODY_H
#define REMOTE_BODY_H

#include "node/inode.h"

namespace Shabby::Node {

class RemoteBody : public INode {
public:
  RemoteBody(Vector2 pos);
  ~RemoteBody() = default;

  void Update(float dt) override;
};

} // namespace Shabby::Node

#endif // REMOTE_BODY_H
