#include "raylib.h"

#include "engine/entities/entities.h"
#include "engine/entities/entities_definition.h"

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "shabby");

  SetTargetFPS(60);               

  // Init objects
  entities_t* e = init_entities(ENTITY_PLAYER, "assets/Beaf.png");

  while (!WindowShouldClose())  
  {
    // Update

    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);

    draw_entities(e);

    EndDrawing();
  }

  CloseWindow();        
  return 0;
}
