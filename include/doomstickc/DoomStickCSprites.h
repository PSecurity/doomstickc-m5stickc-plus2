#pragma once

#include <Arduino.h>
#include <M5Unified.h>
#include "doomstickc/DoomStickCEnemies.h"

/*
  DoomStickCSprites
  v4.0 Full Evolution

  Adiciona visual por tipo de inimigo:
  - Normal : vermelho (original)
  - Fast   : laranja/amarelo, menor
  - Tank   : cinza/verde escuro, maior
  - Ranged : roxo/azul, com "olho" brilhante
*/

namespace DoomStickCSprites {
  uint16_t pickupColor(lgfx::LGFX_Device& display, char pickupType);

  int pickupSize(float distance);

  int enemySpriteHeight(float distance, DoomStickCEnemies::EnemyType type);
  int enemySpriteWidth(int spriteHeight, DoomStickCEnemies::EnemyType type);
  int enemyBob(float anim);

  // Cores por tipo
  uint16_t enemyBodyColor(lgfx::LGFX_Device& display, DoomStickCEnemies::EnemyType type);
  uint16_t enemyBodyAltColor(lgfx::LGFX_Device& display, DoomStickCEnemies::EnemyType type);
  uint16_t enemyDarkColor(lgfx::LGFX_Device& display, DoomStickCEnemies::EnemyType type);
  uint16_t enemyEyeColor(lgfx::LGFX_Device& display, DoomStickCEnemies::EnemyType type);
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
    int spriteH,
    DoomStickCEnemies::EnemyType type,
    int hp
  );
}
