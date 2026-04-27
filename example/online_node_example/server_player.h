#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "node/collision_shape/rigid_body.h"
#include "node/hitbox/rectangle_hitbox.h"
#include "core/physics/icollider.h"
#include <cmath>

class ServerPlayer : public Shabby::Node::RigidBody {
public:
  static constexpr float SPEED       = 120.0f;
  static constexpr float WORLD_MIN_X = -480.0f;
  static constexpr float WORLD_MAX_X = 1560.0f;
  static constexpr float WORLD_MIN_Y = -412.0f;
  static constexpr float WORLD_MAX_Y =  988.0f;

  ServerPlayer(Vector2 pos)
    : Shabby::Node::RigidBody(pos)
  {}

  void SetInput(float dx, float dy)
  {
    _input_dx = dx;
    _input_dy = dy;
    if (dx != 0 || dy != 0) {
      float len = sqrtf(dx * dx + dy * dy);
      _face_dx = dx / len;
      _face_dy = dy / len;
    }
  }

  float GetInputDx() const { return _input_dx; }
  float GetInputDy() const { return _input_dy; }
  float GetFaceDx()  const { return _face_dx; }
  float GetFaceDy()  const { return _face_dy; }

  void _physics_process(float dt) override
  {
    (void)dt;
    float dx = _input_dx;
    float dy = _input_dy;
    float len = sqrtf(dx * dx + dy * dy);
    if (len > 0) { dx /= len; dy /= len; }
    _dir = {dx * SPEED, dy * SPEED};

    if (_pos.x <= WORLD_MIN_X && _dir.x < 0) _dir.x = 0;
    if (_pos.x >= WORLD_MAX_X && _dir.x > 0) _dir.x = 0;
    if (_pos.y <= WORLD_MIN_Y && _dir.y < 0) _dir.y = 0;
    if (_pos.y >= WORLD_MAX_Y && _dir.y > 0) _dir.y = 0;
  }

private:
  float _input_dx = 0;
  float _input_dy = 0;
  float _face_dx  = 1;
  float _face_dy  = 0;
};

#endif // SERVER_PLAYER_H
