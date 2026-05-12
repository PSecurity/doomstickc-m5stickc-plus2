#include "doomstickc/DoomStickCSprites.h"

namespace DoomStickCSprites {
  uint16_t pickupColor(lgfx::LGFX_Device& display, char pickupType) {
    if (pickupType == 'H') {
      return display.color565(80, 255, 120);
    }

    if (pickupType == 'M') {
      return display.color565(80, 150, 255);
    }

    if (pickupType == 'E') {
      return display.color565(250, 230, 80);
    }

    if (pickupType == 'B') {
      return display.color565(190, 90, 255);
    }

    return display.color565(255, 255, 255);
  }

  int pickupSize(float distance) {
    int size = static_cast<int>(31.0f / distance);

    if (size < 4) {
      size = 4;
    }

    if (size > 19) {
      size = 19;
    }

    return size;
  }

  int enemySpriteHeight(float distance) {
    int spriteH = static_cast<int>(70.0f / distance);

    if (spriteH < 6) {
      spriteH = 6;
    }

    if (spriteH > 60) {
      spriteH = 60;
    }

    return spriteH;
  }

  int enemySpriteWidth(int spriteHeight) {
    return spriteHeight / 2;
  }

  int enemyBob(float anim) {
    return static_cast<int>(sinf(anim) * 2.0f);
  }

  uint16_t enemyBodyColor(lgfx::LGFX_Device& display) {
    return display.color565(165, 55, 50);
  }

  uint16_t enemyBodyAltColor(lgfx::LGFX_Device& display) {
    return display.color565(120, 35, 35);
  }

  uint16_t enemyDarkColor(lgfx::LGFX_Device& display) {
    return display.color565(60, 18, 20);
  }

  uint16_t enemyEyeColor(lgfx::LGFX_Device& display) {
    return display.color565(255, 235, 90);
  }

  uint16_t enemyShadowColor(lgfx::LGFX_Device& display) {
    return display.color565(18, 12, 18);
  }

  void drawPickupSprite(
    M5Canvas& canvas,
    lgfx::LGFX_Device& display,
    int sx,
    int sy,
    int size,
    char pickupType
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
    int sx,
    int sy,
    int spriteW,
    int spriteH
  ) {
    uint16_t body = enemyBodyColor(display);
    uint16_t body2 = enemyBodyAltColor(display);
    uint16_t dark = enemyDarkColor(display);
    uint16_t eye = enemyEyeColor(display);

    canvas.fillEllipse(sx, sy + spriteH + 3, max(3, spriteW), 3, enemyShadowColor(display));
    canvas.fillRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, body);
    canvas.drawRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, dark);
    canvas.fillRect(sx - spriteW / 2 + 2, sy + spriteH / 2, max(2, spriteW - 4), spriteH / 3, body2);
    canvas.fillCircle(sx - spriteW / 4, sy + spriteH / 4, 2, eye);
    canvas.fillCircle(sx + spriteW / 4, sy + spriteH / 4, 2, eye);
    canvas.drawFastHLine(sx - spriteW / 3, sy + spriteH / 2, max(2, spriteW * 2 / 3), dark);
    canvas.drawLine(sx - spriteW / 2, sy + 4, sx - spriteW, sy - 5, dark);
    canvas.drawLine(sx + spriteW / 2, sy + 4, sx + spriteW, sy - 5, dark);
    canvas.drawLine(sx - spriteW / 2, sy + spriteH / 2, sx - spriteW, sy + spriteH / 2 + 8, dark);
    canvas.drawLine(sx + spriteW / 2, sy + spriteH / 2, sx + spriteW, sy + spriteH / 2 + 8, dark);
  }
}
