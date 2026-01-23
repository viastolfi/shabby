#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdlib.h>

#include "entities_definition.h"
#include "../core/sprite/sprite.h"

entities_t* init_entities(
    entities_kind kind, 
    const char* texture_path);
void draw_entities(entities_t* e);

#endif // ENTITIES_H
