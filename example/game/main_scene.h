#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "scene/scene.h"

namespace game {

class MainScene : public engine::Scene {
public:
  explicit MainScene();
  ~MainScene() override = default;
};

} // namespace game

#endif // MAIN_SCENE_H
