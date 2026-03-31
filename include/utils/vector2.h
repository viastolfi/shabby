#ifndef VECTOR2_H
#define VECTOR2_H

#include <functional>
#include "raylib.h"
#include "raymath.h"

namespace Shabby {

inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
  return { v1.x + v2.x, v1.y + v2.y };
}

inline Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
  v1.x += v2.x;
  v1.y += v2.y;
  return v1;
}

} // namespace Shabby

namespace std {

template<>
struct hash<Vector2>
{
  size_t operator()(const Vector2& v) const noexcept
  {
    size_t hx = std::hash<float>{}(v.x);
    size_t hy = std::hash<float>{}(v.y);
    return hx ^ (hy << 1);
  }
};
}

#endif // VECTOR2_H
