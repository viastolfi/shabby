#ifndef SCENE_H
#define SCENE_H

#include "node/inode.h"

namespace Shabby::Node {

class Scene : public INode {
public:
  Scene() = default;
  ~Scene() = default;
};

} // namespace Shabby::Node

#endif // SCENE_H
