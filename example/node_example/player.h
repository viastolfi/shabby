#ifndef PLAYER_H
#define PLAYER_H

#include "node/rigid_body.h"

class Player : public Shabby::Node::RigidBody {
public:
  Player(Vector2 pos)
    :Shabby::Node::RigidBody(pos)
  {}

  ~Player() = default;
};

#endif // PLAYER_H
