#include "physics/hitbox/rectangle_hitbox.h"

namespace engine {

RectangleHitbox::RectangleHitbox(Rectangle shape)
  : _shape(shape)
{}

RectangleHitbox::~RectangleHitbox()
{}

void RectangleHitbox::Draw() 
{
  DrawLine(_shape.x, _shape.y, _shape.x + _shape.width, _shape.y, BLUE);
  DrawLine(_shape.x, _shape.y, _shape.x, _shape.y + _shape.height, BLUE);
  DrawLine(_shape.x + _shape.width, _shape.y, _shape.x + _shape.width, _shape.y + _shape.height, BLUE); 
  DrawLine(_shape.x, _shape.y + _shape.height, _shape.x + _shape.width, _shape.y + _shape.height, BLUE);
}

void RectangleHitbox::Update(Vector2 new_pos) 
{
  _shape.x = new_pos.x;
  _shape.y = new_pos.y;
}

} // namespace engine
