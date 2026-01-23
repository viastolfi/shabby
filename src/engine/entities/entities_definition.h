#ifndef ENTITIES_DEFINITION_H
#define ENTITIES_DEFINITION_H

#include "../core/sprite/sprite.h"

typedef enum 
{
  ENTITY_PLAYER,
} entities_kind;

typedef struct 
{
  entities_kind kind; 

  sprite_t* sprite;

  /*
  union 
  {
  
  }
  */

} entities_t;

#endif // ENTITIES_DEFINITION_H
