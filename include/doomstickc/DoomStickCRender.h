#pragma once

#include <M5Unified.h>

/*
  DoomStickCRender
  v2.9 Raycasting Render Split

  Centralizes safe rendering helpers.

  This module is still conservative:
  - Full raycasting loop remains in main.cpp for safety.
  - Shared sky/floor, wall shade and wall color helpers are now centralized.
  - Gameplay, timing, input and audio remain untouched.
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

  void drawSkyAndFloor(
    M5Canvas& canvas,
    int screenW,
    int screenH,
    int topOffset,
    uint16_t skyColor,
    uint16_t floorColor,
    uint16_t floorLineColor
  );

  uint8_t computeWallShade(float correctedDistance);

  uint16_t makeWallColor(
    lgfx::LGFX_Device& display,
    char hitCell,
    uint8_t shade,
    int currentLevel
  );
}
