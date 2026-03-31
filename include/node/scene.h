#ifndef SCENE_H
#define SCENE_H

#include "node/inode.h"

namespace Shabby::Node {

class Scene : public INode {
public:
  Scene();
  ~Scene() = default;

  void Draw() override;
  void Update(float dt) override;
};

} // namespace Shabby::Node

#endif // SCENE_H
