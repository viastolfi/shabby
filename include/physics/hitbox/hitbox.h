#ifndef HITBOX_H
#define HITBOX_H

namespace engine {

class Hitbox {
public:
  explicit Hitbox();
  ~Hitbox() = 0;
private:
  uint64_t _id;
};

} // namespace engine

#endif // HITBOX_H
