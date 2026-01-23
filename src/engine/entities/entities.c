#include "entities.h"

entities_t* init_entities(
    entities_kind kind,
    const char* texture_path)
{
  entities_t* e = calloc(1, sizeof(entities_t));
  if (!e) {
    return NULL;  
  }

  e->kind = kind;
  e->sprite = init_sprite(texture_path);
  if (!e->sprite) {
    return NULL; 
  }

  return e;
}

void draw_entities(entities_t* e)
{
  draw_sprite(e->sprite);
}
