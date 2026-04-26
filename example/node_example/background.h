#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "core/drawable/idrawable.h"
#include "raylib.h"

class Background : public Shabby::Core::IDrawable {
public:
  void Draw() override
  {
    DrawRectangle(-5000, -5000, 10000, 10000, Color{ 55, 95, 20, 255 });
  }
};

#endif // BACKGROUND_H
