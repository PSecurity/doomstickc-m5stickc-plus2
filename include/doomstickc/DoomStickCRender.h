#pragma once

#include <M5Unified.h>

/*
  DoomStickCRender
  v2.7 Render Foundation

  Centralizes small, safe rendering helpers.

  This module is intentionally conservative:
  - It does not move raycasting yet.
  - It does not change gameplay, timing, input or audio.
  - It only owns reusable visual helpers already validated in main.cpp.
*/

namespace DoomStickCRender {
  void drawFrameBorder(
    M5Canvas& canvas,
    int screenW,
    int screenH,
    uint16_t outerColor,
    uint16_t innerColor
  );

  void drawCyberGrid(
    M5Canvas& canvas,
    int screenW,
    int screenH,
    uint16_t gridColor
  );
}
