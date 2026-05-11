#include "doomstickc/DoomStickCRender.h"

namespace DoomStickCRender {
  void drawFrameBorder(
    M5Canvas& canvas,
    int screenW,
    int screenH,
    uint16_t outerColor,
    uint16_t innerColor
  ) {
    canvas.drawRect(0, 0, screenW, screenH, outerColor);
    canvas.drawRect(1, 1, screenW - 2, screenH - 2, innerColor);
  }

  void drawCyberGrid(
    M5Canvas& canvas,
    int screenW,
    int screenH,
    uint16_t gridColor
  ) {
    for (int x = 0; x < screenW; x += 20) {
      canvas.drawFastVLine(x, 0, screenH, gridColor);
    }

    for (int y = 0; y < screenH; y += 15) {
      canvas.drawFastHLine(0, y, screenW, gridColor);
    }
  }
}
