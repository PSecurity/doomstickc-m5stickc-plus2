#pragma once

/*
  DoomStickCPlayer
  v2.2 Player Module

  Centralizes player state, defaults and safe stat helpers.

  This module is intentionally conservative:
  - It preserves validated v2.1 movement/gameplay behavior.
  - It does not change controls, speed, tilt, collision or render.
  - It prepares future extraction of movement and collision helpers.
*/

namespace DoomStickCPlayer {
  static constexpr int PLAYER_MAX_HP = 100;
  static constexpr int PLAYER_START_AMMO = 24;
  static constexpr int PLAYER_LEVEL_HP_BONUS = 18;
  static constexpr int PLAYER_LEVEL_AMMO_BONUS = 8;

  static constexpr float START_X = 2.5f;
  static constexpr float START_Y = 2.5f;
  // v3.0 fix:
  // PI/2 makes the player start facing down the map corridor instead of a wall.
  static constexpr float START_ANGLE = 1.5707963f;

  struct PlayerState {
    float x;
    float y;
    float a;
    int hp;
    int ammo;
  };

  inline void resetPosition(PlayerState& player) {
    player.x = START_X;
    player.y = START_Y;
    player.a = START_ANGLE;
  }

  inline void resetStats(PlayerState& player) {
    player.hp = PLAYER_MAX_HP;
    player.ammo = PLAYER_START_AMMO;
  }

  inline void applyLevelBonus(PlayerState& player) {
    player.hp += PLAYER_LEVEL_HP_BONUS;
    if (player.hp > PLAYER_MAX_HP) {
      player.hp = PLAYER_MAX_HP;
    }
    player.ammo += PLAYER_LEVEL_AMMO_BONUS;
  }

  inline void applyLevelBonus(PlayerState& player, int hpBonus, int ammoBonus) {
    player.hp += hpBonus;
    if (player.hp > PLAYER_MAX_HP) {
      player.hp = PLAYER_MAX_HP;
    }
    player.ammo += ammoBonus;
  }

  inline bool isLowHp(const PlayerState& player) {
    return player.hp > 0 && player.hp <= 25;
  }
}
