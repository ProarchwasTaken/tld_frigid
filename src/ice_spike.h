#include <raylib.h>
#include "solid_tile.h"
#include "player.h"


class IceSpike : public Tile {
public:
  IceSpike(int tileX, int tileY);
  ~IceSpike();

  Rectangle detection_rect;

  void draw();

private:
  Texture2D texture;
  Vector2 position;
};
