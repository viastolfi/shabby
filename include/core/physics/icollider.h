#ifndef ICOLLIDER_H
#define ICOLLIDER_H

namespace Shabby::Core {

class ICollider {
public:
  virtual ~ICollider() = default;

  // TODO: use abstract Shape class instead
  virtual Rectangle GetShape() const = 0;
  virtual bool GetIsEnteredState() const = 0;
  virtual const Vector2 GetPosition() const = 0;

  virtual void OnEnter(ICollider* other) = 0;
  virtual void OnExit(ICollider* other) = 0;
};

} // namespace Shabby::Core

#endif // ICOLLIDER_H
