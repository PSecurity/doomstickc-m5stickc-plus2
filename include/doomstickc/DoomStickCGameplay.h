#pragma once

#include <Arduino.h>

/*
  DoomStickCGameplay
  v4.0 Full Evolution

  - Modos de dificuldade: Easy / Normal / Hard
  - Score: kills + tempo
  - Balanceamento por fase e dificuldade
*/

namespace DoomStickCGameplay {

  // ---- Dificuldade ----
  enum class Difficulty : uint8_t {
    Easy   = 0,
    Normal = 1,
    Hard   = 2
  };

  static constexpr const char* DIFFICULTY_LABELS[] = { "EASY", "NORMAL", "HARD" };
  static constexpr const char* POLISH_STAGE         = "Full Evolution";
  static constexpr const char* DIFFICULTY_LABEL     = "NORMAL";
  static constexpr unsigned long LEVEL_START_PAUSE_MS = 900;

  // Dificuldade atual (alterada na tela inicial)
  extern Difficulty currentDifficulty;

  // ---- Score ----
  struct ScoreState {
    int   totalKills;
    int   levelKills;
    uint32_t levelStartMs;
    uint32_t totalTimeMs;
    uint32_t levelTimeMs;
  };

  extern ScoreState score;

  inline void resetScore() {
    score.totalKills  = 0;
    score.levelKills  = 0;
    score.levelStartMs = 0;
    score.totalTimeMs  = 0;
    score.levelTimeMs  = 0;
  }

  inline void startLevelTimer(uint32_t nowMs) {
    score.levelStartMs = nowMs;
    score.levelKills   = 0;
  }

  inline void finishLevelTimer(uint32_t nowMs) {
    score.levelTimeMs  = nowMs - score.levelStartMs;
    score.totalTimeMs += score.levelTimeMs;
  }

  inline void registerKill() {
    score.totalKills++;
    score.levelKills++;
  }

  // ---- Multiplicadores de dificuldade ----
  inline float difficultyDamageMultiplier() {
    switch (currentDifficulty) {
      case Difficulty::Easy: return 0.60f;
      case Difficulty::Hard: return 1.55f;
      default:               return 1.00f;
    }
  }

  inline float difficultySpeedMultiplier() {
    switch (currentDifficulty) {
      case Difficulty::Easy: return 0.70f;
      case Difficulty::Hard: return 1.35f;
      default:               return 1.00f;
    }
  }

  inline int difficultyStartAmmo() {
    switch (currentDifficulty) {
      case Difficulty::Easy: return 36;
      case Difficulty::Hard: return 16;
      default:               return 24;
    }
  }

  // ---- Balanceamento por fase ----
  inline int enemyDamageForLevel(int levelIndex) {
    int base;
    if      (levelIndex <= 0) base = 6;
    else if (levelIndex == 1) base = 7;
    else if (levelIndex == 2) base = 8;
    else if (levelIndex == 3) base = 10;
    else                      base = 13;
    return static_cast<int>(base * difficultyDamageMultiplier());
  }

  inline float enemySpeedForLevel(int levelIndex) {
    float base;
    if      (levelIndex <= 0) base = 0.38f;
    else if (levelIndex == 1) base = 0.42f;
    else if (levelIndex == 2) base = 0.47f;
    else if (levelIndex == 3) base = 0.52f;
    else                      base = 0.58f;
    return base * difficultySpeedMultiplier();
  }

  inline int hpBonusForNextLevel(int levelIndex) {
    if (levelIndex <= 0) return 20;
    if (levelIndex == 1) return 16;
    if (levelIndex == 2) return 12;
    return 8;
  }

  inline int ammoBonusForNextLevel(int levelIndex) {
    if (levelIndex <= 0) return 10;
    if (levelIndex == 1) return 8;
    if (levelIndex == 2) return 6;
    return 5;
  }

  // ---- Score final ----
  // Retorna pontuação: kills * 100 + bônus de tempo
  inline int computeFinalScore() {
    int killScore = score.totalKills * 100;
    // Bônus de tempo: quanto mais rápido, mais pontos (máx 5000)
    uint32_t totalSec = score.totalTimeMs / 1000;
    int timeBonus = (totalSec < 300) ? (int)(5000 - totalSec * 15) : 0;
    if (timeBonus < 0) timeBonus = 0;
    return killScore + timeBonus;
  }
}
