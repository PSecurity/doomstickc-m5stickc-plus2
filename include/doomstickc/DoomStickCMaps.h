#pragma once

#include <Arduino.h>

/*
  DoomStickCMaps
  v4.0 Full Evolution

  5 fases, 8 inimigos por fase, tipos de inimigo no spawn.
  Sala secreta: célula 'W' = parede falsa (abre com Power).
  Pickup 'S' = Shotgun.

  Cell legend:
  # = parede
  . = chão
  D = porta
  H = pickup vida
  M = pickup munição
  E = saída/fim da fase
  B = pickup Blaster
  S = pickup Shotgun
  W = parede falsa (sala secreta)
*/

namespace DoomStickCMaps {
  static constexpr int MAP_W       = 20;
  static constexpr int MAP_H       = 16;
  static constexpr int LEVEL_COUNT = 5;
  static constexpr int ENEMY_COUNT = 8;

  struct EnemySpawn {
    float x;
    float y;
    bool  alive;
    float anim;
    uint8_t type;   // 0=Normal 1=Fast 2=Tank 3=Ranged
  };

  extern const char       LEVEL_MAPS[LEVEL_COUNT][MAP_H][MAP_W + 1];
  extern const EnemySpawn LEVEL_ENEMIES[LEVEL_COUNT][ENEMY_COUNT];
}
