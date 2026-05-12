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

  void drawSkyAndFloor(
    M5Canvas& canvas,
    int screenW,
    int screenH,
    int topOffset,
    uint16_t skyColor,
    uint16_t floorColor,
    uint16_t floorLineColor
  ) {
    canvas.fillRect(0, topOffset, screenW, (screenH - topOffset) / 2, skyColor);
    canvas.fillRect(0, topOffset + (screenH - topOffset) / 2, screenW, (screenH - topOffset) / 2, floorColor);

    for (int y = screenH / 2 + 10; y < screenH; y += 10) {
      canvas.drawFastHLine(0, y, screenW, floorLineColor);
    }
  }

  uint8_t computeWallShade(float correctedDistance) {
    float shadeF = 218.0f - correctedDistance * 25.0f;

    if (shadeF < 18.0f) {
      shadeF = 18.0f;
    }

    if (shadeF > 240.0f) {
      shadeF = 240.0f;
    }

    return static_cast<uint8_t>(shadeF);
  }

  uint16_t makeWallColor(
    lgfx::LGFX_Device& display,
    char hitCell,
    uint8_t shade,
    int currentLevel
  ) {
    if (hitCell == 'D') {
      return display.color565(shade, static_cast<uint8_t>(shade * 0.58f), 30);
    }

    uint8_t phase = static_cast<uint8_t>(currentLevel * 16);
    return display.color565(
      static_cast<uint8_t>(shade * 0.40f + phase),
      static_cast<uint8_t>(shade * 0.34f),
      shade
    );
  }
}
