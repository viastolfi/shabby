#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "core/drawable/idrawable.h"
#include "raylib.h"

class Background : public Shabby::Core::IDrawable {
public:
  void Draw() override
  {
    // Large world-space gradient — 4 distinct corner colours so movement
    // is immediately visible in every direction.
    DrawRectangleGradientEx(
      Rectangle{ -2000, -2000, 4000, 4000 },
      Color{  80, 140, 255, 255 },   // top-left     : blue
      Color{ 100, 210, 120, 255 },   // bottom-left  : green
      Color{ 255, 180,  80, 255 },   // top-right    : orange
      Color{ 220,  80, 200, 255 }    // bottom-right : purple
    );
  }
};

#endif // BACKGROUND_H
