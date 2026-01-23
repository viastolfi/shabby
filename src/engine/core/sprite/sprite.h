#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>

#include "raylib.h"

typedef struct 
{
  Texture2D texture;
} sprite_t;

inline static sprite_t* init_sprite(const char* path) 
{
  sprite_t* s = calloc(1, sizeof(sprite_t));
  if (!s) {
    return NULL; 
  }

  s->texture = LoadTexture(path);

  return s;
}

#define draw_sprite(s) \
  do { \
    DrawTexture(s->texture, 100, 100, WHITE); \
  } while(0) 

#endif // SPRITE_H
