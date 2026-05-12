#pragma once

/*
  DoomStickCMaps
  v2.1 Maps Module

  Centralizes internal map data and enemy spawn data.

  This module is data-only for now.

  Cell legend update:
  B = weapon pickup / Blaster
  It preserves the v2.0 maps, pickups, doors, exits and enemy placements.
*/

namespace DoomStickCMaps {
  static constexpr int MAP_W = 16;
  static constexpr int MAP_H = 16;
  static constexpr int LEVEL_COUNT = 3;
  static constexpr int ENEMY_COUNT = 4;

  struct EnemySpawn {
    float x;
    float y;
    bool alive;
    float anim;
  };

  extern const char LEVEL_MAPS[LEVEL_COUNT][MAP_H][MAP_W + 1];
  extern const EnemySpawn LEVEL_ENEMIES[LEVEL_COUNT][ENEMY_COUNT];
}
