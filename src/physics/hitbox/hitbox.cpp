#include "physics/hitbox/hitbox.h"

namespace engine {

Hitbox::~Hitbox()
{}

void Hitbox::SetId(uint64_t id)
{
  id = _id;
}

} // namespace engine
