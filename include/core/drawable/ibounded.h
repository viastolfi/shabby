#ifndef IBOUNDED_H
#define IBOUNDED_H

#include "raylib.h"

namespace Shabby::Core {

class IBounded {
public:
  virtual ~IBounded() = default;
  virtual Rectangle GetBounds() const = 0;
};

} // namespace Shabby::Core

#endif // IBOUNDED_H
