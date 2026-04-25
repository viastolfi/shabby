#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include "node/inode.h"
#include "core/drawable/idrawable.h"
#include "raylib.h"

class StaticEntity : public Shabby::Node::INode, public Shabby::Core::IDrawable {
public:
  StaticEntity(Vector2 pos, Vector2 size, Color color)
    : Shabby::Node::INode(pos), _size(size), _color(color)
  {}

  void Draw() override
  {
    DrawRectangleV(_pos, _size, _color);
  }

private:
  Vector2 _size;
  Color   _color;
};

#endif // STATIC_ENTITY_H
