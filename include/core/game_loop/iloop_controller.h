#ifndef ILOOP_CONTROLLER_H
#define ILOOP_CONTROLLER_H

namespace Shabby::Core {

class ILoopController {
public:
  virtual ~ILoopController() = default;
  virtual bool ShouldContinue() = 0;
};

} // namespace Shabby::Core

#endif // ILOOP_CONTROLLER_H

