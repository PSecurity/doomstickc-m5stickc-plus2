#pragma once

#include <Arduino.h>
#include "doomstickc/DoomStickCMaps.h"

/*
  DoomStickCEnemies
  v2.3 Enemies Module

  Centralizes enemy state, combat constants and small enemy helpers.

  This module is intentionally conservative:
  - It preserves validated v2.2 enemy behavior.
  - It does not change chase logic, hit detection, rendering or level flow.
  - Future versions may move full enemy AI here.
*/

namespace DoomStickCEnemies {
  static constexpr int ENEMY_DAMAGE = 7;
  static constexpr uint32_t ENEMY_DAMAGE_COOLDOWN_MS = 850;
  static constexpr float ENEMY_SPEED = 0.42f;
  static constexpr float ENEMY_ATTACK_DISTANCE = 0.55f;
  static constexpr float ENEMY_ANIM_SPEED = 5.0f;

  struct EnemyState {
    float x;
    float y;
    bool alive;
    uint32_t lastDamageMs;
    float anim;
  };

  inline void loadFromSpawn(EnemyState& enemy, const DoomStickCMaps::EnemySpawn& spawn) {
    enemy.x = spawn.x;
    enemy.y = spawn.y;
    enemy.alive = spawn.alive;
    enemy.lastDamageMs = 0;
    enemy.anim = spawn.anim;
  }

  inline void updateAnimation(EnemyState& enemy, float dt) {
    if (!enemy.alive) return;
    enemy.anim += dt * ENEMY_ANIM_SPEED;
  }

  inline bool canDamage(const EnemyState& enemy, uint32_t nowMs) {
    return nowMs - enemy.lastDamageMs > ENEMY_DAMAGE_COOLDOWN_MS;
  }

  inline void markDamage(EnemyState& enemy, uint32_t nowMs) {
    enemy.lastDamageMs = nowMs;
  }

  inline int countAlive(const EnemyState* enemies, int count) {
    int alive = 0;
    for (int i = 0; i < count; i++) {
      if (enemies[i].alive) {
        alive++;
      }
    }
    return alive;
  }
}
