#pragma once

#include <Arduino.h>
#include <M5Unified.h>

/*
  DoomStickCSprites
  v3.4 Full Sprite Drawing Extraction

  Centralizes safe visual helpers and complete sprite drawing for pickups/enemies.

  This module is intentionally conservative:
  - It does not change enemy AI, hit detection, damage or gameplay.
  - It only moves already validated drawing code out of main.cpp.
*/

namespace DoomStickCSprites {
  uint16_t pickupColor(lgfx::LGFX_Device& display, char pickupType);

  int pickupSize(float distance);

  int enemySpriteHeight(float distance);
  int enemySpriteWidth(int spriteHeight);
  int enemyBob(float anim);

  uint16_t enemyBodyColor(lgfx::LGFX_Device& display);
  uint16_t enemyBodyAltColor(lgfx::LGFX_Device& display);
  uint16_t enemyDarkColor(lgfx::LGFX_Device& display);
  uint16_t enemyEyeColor(lgfx::LGFX_Device& display);
  uint16_t enemyShadowColor(lgfx::LGFX_Device& display);

  void drawPickupSprite(
    M5Canvas& canvas,
    lgfx::LGFX_Device& display,
    int sx,
    int sy,
    int size,
    char pickupType
  );

  void drawEnemySprite(
    M5Canvas& canvas,
    lgfx::LGFX_Device& display,
    int sx,
    int sy,
    int spriteW,
    int spriteH
  );
}
