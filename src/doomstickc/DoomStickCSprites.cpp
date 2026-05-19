#include "doomstickc/DoomStickCSprites.h"

namespace DoomStickCSprites {

  using EType = DoomStickCEnemies::EnemyType;

  uint16_t pickupColor(lgfx::LGFX_Device& display, char pickupType) {
    if (pickupType == 'H') return display.color565(80, 255, 120);
    if (pickupType == 'M') return display.color565(80, 150, 255);
    if (pickupType == 'E') return display.color565(250, 230, 80);
    if (pickupType == 'B') return display.color565(190, 90, 255);
    if (pickupType == 'S') return display.color565(255, 180, 60);   // Shotgun — laranja
    return display.color565(255, 255, 255);
  }

  int pickupSize(float distance) {
    int size = static_cast<int>(31.0f / distance);
    if (size < 4)  size = 4;
    if (size > 19) size = 19;
    return size;
  }

  int enemySpriteHeight(float distance, EType type) {
    float scale = 70.0f;
    if (type == EType::Tank)  scale = 90.0f;   // Tank é maior
    if (type == EType::Fast)  scale = 55.0f;   // Fast é menor
    int spriteH = static_cast<int>(scale / distance);
    if (spriteH < 6)  spriteH = 6;
    if (spriteH > 72) spriteH = 72;
    return spriteH;
  }

  int enemySpriteWidth(int spriteHeight, EType type) {
    if (type == EType::Tank) return spriteHeight * 2 / 3;  // Tank mais largo
    if (type == EType::Fast) return spriteHeight / 3;      // Fast mais estreito
    return spriteHeight / 2;
  }

  int enemyBob(float anim) {
    return static_cast<int>(sinf(anim) * 2.0f);
  }

  uint16_t enemyBodyColor(lgfx::LGFX_Device& display, EType type) {
    switch (type) {
      case EType::Fast:   return display.color565(220, 140, 20);   // laranja
      case EType::Tank:   return display.color565(55, 90, 55);     // verde escuro
      case EType::Ranged: return display.color565(80, 50, 160);    // roxo
      default:            return display.color565(165, 55, 50);    // vermelho
    }
  }

  uint16_t enemyBodyAltColor(lgfx::LGFX_Device& display, EType type) {
    switch (type) {
      case EType::Fast:   return display.color565(160, 90, 10);
      case EType::Tank:   return display.color565(35, 65, 35);
      case EType::Ranged: return display.color565(55, 30, 120);
      default:            return display.color565(120, 35, 35);
    }
  }

  uint16_t enemyDarkColor(lgfx::LGFX_Device& display, EType type) {
    switch (type) {
      case EType::Fast:   return display.color565(60, 35, 5);
      case EType::Tank:   return display.color565(15, 28, 15);
      case EType::Ranged: return display.color565(20, 10, 50);
      default:            return display.color565(60, 18, 20);
    }
  }

  uint16_t enemyEyeColor(lgfx::LGFX_Device& display, EType type) {
    switch (type) {
      case EType::Fast:   return display.color565(255, 255, 80);   // amarelo
      case EType::Tank:   return display.color565(255, 80, 80);    // vermelho
      case EType::Ranged: return display.color565(80, 220, 255);   // ciano brilhante
      default:            return display.color565(255, 235, 90);
    }
  }

  uint16_t enemyShadowColor(lgfx::LGFX_Device& display) {
    return display.color565(18, 12, 18);
  }

  void drawPickupSprite(
    M5Canvas& canvas,
    lgfx::LGFX_Device& display,
    int sx, int sy, int size, char pickupType
  ) {
    uint16_t col = pickupColor(display, pickupType);
    canvas.drawCircle(sx, sy, size / 2 + 2, col);
    canvas.fillRoundRect(sx - size / 2, sy - size / 2, size, size, 3, col);
    canvas.setTextDatum(middle_center);
    canvas.setTextSize(1);
    canvas.setTextColor(display.color565(0, 0, 0), col);
    canvas.drawString(String(pickupType), sx, sy);
    canvas.setTextDatum(top_left);
  }

  void drawEnemySprite(
    M5Canvas& canvas,
    lgfx::LGFX_Device& display,
    int sx, int sy, int spriteW, int spriteH,
    EType type, int hp
  ) {
    uint16_t body  = enemyBodyColor(display, type);
    uint16_t body2 = enemyBodyAltColor(display, type);
    uint16_t dark  = enemyDarkColor(display, type);
    uint16_t eye   = enemyEyeColor(display, type);

    canvas.fillEllipse(sx, sy + spriteH + 3, max(3, spriteW), 3, enemyShadowColor(display));
    canvas.fillRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, body);
    canvas.drawRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, dark);
    canvas.fillRect(sx - spriteW / 2 + 2, sy + spriteH / 2, max(2, spriteW - 4), spriteH / 3, body2);

    // Olhos — Ranged tem olho único grande (cíclope)
    if (type == EType::Ranged) {
      canvas.fillCircle(sx, sy + spriteH / 4, 3, eye);
      canvas.fillCircle(sx, sy + spriteH / 4, 1, display.color565(255, 255, 255));
    } else {
      canvas.fillCircle(sx - spriteW / 4, sy + spriteH / 4, 2, eye);
      canvas.fillCircle(sx + spriteW / 4, sy + spriteH / 4, 2, eye);
    }

    canvas.drawFastHLine(sx - spriteW / 3, sy + spriteH / 2, max(2, spriteW * 2 / 3), dark);
    canvas.drawLine(sx - spriteW / 2, sy + 4, sx - spriteW, sy - 5, dark);
    canvas.drawLine(sx + spriteW / 2, sy + 4, sx + spriteW, sy - 5, dark);
    canvas.drawLine(sx - spriteW / 2, sy + spriteH / 2, sx - spriteW, sy + spriteH / 2 + 8, dark);
    canvas.drawLine(sx + spriteW / 2, sy + spriteH / 2, sx + spriteW, sy + spriteH / 2 + 8, dark);

    // Barra de HP para Tank (tem mais de 1 HP)
    if (type == EType::Tank && hp > 0) {
      int barW = spriteW;
      int barX = sx - barW / 2;
      int barY = sy - 6;
      canvas.drawRect(barX, barY, barW, 4, dark);
      int filled = (hp * barW) / DoomStickCEnemies::HP_TANK;
      if (filled > 0) {
        canvas.fillRect(barX + 1, barY + 1, filled - 1, 2, display.color565(80, 255, 80));
      }
    }

    // Ranged: "antena" indicando que atira
    if (type == EType::Ranged) {
      canvas.drawFastVLine(sx, sy - 8, 8, eye);
      canvas.fillCircle(sx, sy - 9, 2, eye);
    }
  }
}
