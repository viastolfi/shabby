#ifndef IDRAWABLE_H
#define IDRAWABLE_H

namespace Shabby::Core {

class IDrawable {
public:
  virtual ~IDrawable() = default;
  virtual void Draw() = 0;
};

} // namespace Shabby::Core

#endif // IDRAWABLE_H
