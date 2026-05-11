#pragma once

#include <Arduino.h>

/*
  DoomStickCConfig
  v2.0 Engine Split Foundation

  Central configuration module.

  This is intentionally conservative:
  - It centralizes safe hardware/display/audio flags.
  - It does not change validated gameplay values.
  - It prepares future extraction of game tuning constants.
*/

namespace DoomStickCConfig {
  // Display
  static constexpr int SCREEN_W = 240;
  static constexpr int SCREEN_H = 135;

  // M5StickC Plus2 pin behavior
  static constexpr int PIN_POWER_BUTTON = 35;
  static constexpr int PIN_HOLD = 4;
  static constexpr bool POWER_ACTIVE_LOW = true;

  // Audio
  static constexpr bool AUDIO_ENABLED = true;
  static constexpr uint8_t AUDIO_VOLUME = 110;

  // Build safety
  static constexpr bool USE_INTERNAL_IMU = true;
  static constexpr bool USE_INTERNAL_SPEAKER = AUDIO_ENABLED;
  static constexpr bool USE_INTERNAL_MIC = false;
}
