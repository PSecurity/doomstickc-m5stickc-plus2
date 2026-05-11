#pragma once

#include <Arduino.h>
#include <M5Unified.h>
#include "doomstickc/DoomStickCConfig.h"

/*
  DoomStickCAudio
  v2.6 Audio Module

  Centralizes audio toggle, volume, tone constants and safe playback helper.

  This module is intentionally conservative:
  - It preserves all validated v2.5 audio behavior.
  - It does not change gameplay or timings.
  - It keeps audio easy to disable from one place.
*/

namespace DoomStickCAudio {
  static constexpr bool AUDIO_ENABLED = DoomStickCConfig::AUDIO_ENABLED;
  static constexpr uint8_t AUDIO_VOLUME = DoomStickCConfig::AUDIO_VOLUME;

  static constexpr int TONE_SHOOT = 1800;
  static constexpr int TONE_EMPTY = 180;
  static constexpr int TONE_DAMAGE = 130;
  static constexpr int TONE_PICKUP = 1200;
  static constexpr int TONE_DOOR = 780;
  static constexpr int TONE_START = 920;
  static constexpr int TONE_WIN = 1550;
  static constexpr int TONE_DEAD = 90;
  static constexpr int TONE_LEVEL = 1320;

  void begin();
  void playToneSafe(int freq, int durationMs);
}
