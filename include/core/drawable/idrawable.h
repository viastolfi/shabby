#ifndef IDRAWABLE_H
#define IDRAWABLE_H

namespace Shabby::Core {

class IDrawable {
public:
  virtual ~IDrawable() = default;
  virtual void Draw() = 0;
  virtual void Draw(Vector2 pos) { (void)pos; }

  void SetRenderLayer(int layer) { _render_layer = layer; }
  int  GetRenderLayer() const    { return _render_layer; }

private:
  int _render_layer = 0;
};

} // namespace Shabby::Core

#endif // IDRAWABLE_H
