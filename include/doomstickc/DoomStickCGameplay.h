#pragma once

/*
  DoomStickCGameplay
  v3.2 Difficulty and Balance

  Centralizes safe gameplay balance helpers.

  This module remains conservative:
  - No difficulty menu yet.
  - No control changes.
  - Only light per-level balance values are centralized.
*/

namespace DoomStickCGameplay {
  static constexpr const char* POLISH_STAGE = "Difficulty and Balance";
  static constexpr const char* FIX_START_DIRECTION = "Player starts facing corridor instead of wall";
  static constexpr const char* NEXT_BALANCE_TARGET = "Difficulty modes";
  static constexpr const char* NEXT_FLOW_TARGET = "Cleaner level start experience";

  // Current global profile: NORMAL.
  static constexpr const char* DIFFICULTY_LABEL = "NORMAL";
  static constexpr unsigned long LEVEL_START_PAUSE_MS = 900;

  inline int enemyDamageForLevel(int levelIndex) {
    if (levelIndex <= 0) return 6;
    if (levelIndex == 1) return 7;
    return 8;
  }

  inline float enemySpeedForLevel(int levelIndex) {
    if (levelIndex <= 0) return 0.38f;
    if (levelIndex == 1) return 0.42f;
    return 0.47f;
  }

  inline int hpBonusForNextLevel(int levelIndex) {
    // Level 1 -> 2 gives more help; Level 2 -> 3 gives less.
    if (levelIndex <= 0) return 20;
    return 14;
  }

  inline int ammoBonusForNextLevel(int levelIndex) {
    if (levelIndex <= 0) return 10;
    return 7;
  }
}
