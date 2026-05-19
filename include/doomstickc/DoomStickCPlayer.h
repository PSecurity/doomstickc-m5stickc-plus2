#pragma once

/*
  DoomStickCPlayer
  v4.0 Full Evolution

  Centralizes player state, defaults and safe stat helpers.
  Ammo inicial agora vem de DoomStickCGameplay (dificuldade).
*/

namespace DoomStickCPlayer {
  static constexpr int PLAYER_MAX_HP         = 100;
  static constexpr int PLAYER_START_AMMO     = 24;   // default Normal
  static constexpr int PLAYER_LEVEL_HP_BONUS = 18;
  static constexpr int PLAYER_LEVEL_AMMO_BONUS = 8;

  static constexpr float START_X     = 2.5f;
  static constexpr float START_Y     = 2.5f;
  static constexpr float START_ANGLE = 1.5707963f;  // PI/2 — corredor

  struct PlayerState {
    float x;
    float y;
    float a;
    int   hp;
    int   ammo;
  };

  inline void resetPosition(PlayerState& player) {
    player.x = START_X;
    player.y = START_Y;
    player.a = START_ANGLE;
  }

  inline void resetStats(PlayerState& player, int startAmmo = PLAYER_START_AMMO) {
    player.hp   = PLAYER_MAX_HP;
    player.ammo = startAmmo;
  }

  inline void applyLevelBonus(PlayerState& player, int hpBonus, int ammoBonus) {
    player.hp += hpBonus;
    if (player.hp > PLAYER_MAX_HP) player.hp = PLAYER_MAX_HP;
    player.ammo += ammoBonus;
  }

  inline bool isLowHp(const PlayerState& player) {
    return player.hp > 0 && player.hp <= 25;
  }
}
