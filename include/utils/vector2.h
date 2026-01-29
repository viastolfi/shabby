#ifndef VECTOR2_H
#define  VECTOR2_H

namespace engine {

inline Vector2 operator+(const Vector2 v1, const Vector2 v2)
{
  return {v1.x + v2.x, v1.y + v2.y};
}

} // namespace engine

#endif //  VECTOR2_H
