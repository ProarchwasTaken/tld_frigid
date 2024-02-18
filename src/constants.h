#pragma once
#include <raylib.h>

namespace WINDOW {
  const int WIDTH = 672;
  const int HEIGHT = 384;
}

namespace CANVAS {
  const int WIDTH = 84;
  const int HEIGHT = 48;
}

namespace FRAMERATE {
  const int MAX = 60;
  const int TARGET = 60;
}

namespace COLOR {
  const Color LIGHT = {199, 240, 216, 255};
  const Color DARK = {67, 82, 61, 255};
}

namespace TILE {
  const float SIZE = 4;
  const int ROWS = 21;
  const int COLUMNS = 12;
}
