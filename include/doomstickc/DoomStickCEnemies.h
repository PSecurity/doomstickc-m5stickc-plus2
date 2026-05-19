#pragma once

#include <Arduino.h>
#include "doomstickc/DoomStickCMaps.h"

/*
  DoomStickCEnemies
  v4.0 Full Evolution

  Adds enemy types: Normal, Fast, Tank, Ranged.
  - Normal : comportamento original
  - Fast   : mais rápido, menos HP
  - Tank   : lento, mais HP, dano alto
  - Ranged : para e "atira" de longe (projétil simulado)
*/

namespace DoomStickCEnemies {

  enum class EnemyType : uint8_t {
    Normal = 0,
    Fast   = 1,
    Tank   = 2,
    Ranged = 3
  };

  // Constantes base (Normal)
  static constexpr int   ENEMY_DAMAGE              = 7;
  static constexpr uint32_t ENEMY_DAMAGE_COOLDOWN_MS = 850;
  static constexpr float ENEMY_SPEED               = 0.42f;
  static constexpr float ENEMY_ATTACK_DISTANCE     = 0.55f;
  static constexpr float ENEMY_ANIM_SPEED          = 5.0f;

  // HP por tipo
  static constexpr int HP_NORMAL = 1;   // morre em 1 tiro (comportamento original)
  static constexpr int HP_FAST   = 1;   // fraco mas rápido
  static constexpr int HP_TANK   = 3;   // precisa de 3 tiros
  static constexpr int HP_RANGED = 2;   // médio

  struct EnemyState {
    float    x;
    float    y;
    bool     alive;
    uint32_t lastDamageMs;
    float    anim;
    EnemyType type;
    int      hp;
    // Para inimigo Ranged: cooldown de tiro
    uint32_t lastShotMs;
  };

  // Multiplicadores por tipo
  inline float speedMultiplier(EnemyType t) {
    switch (t) {
      case EnemyType::Fast:   return 1.85f;
      case EnemyType::Tank:   return 0.52f;
      case EnemyType::Ranged: return 0.60f;
      default:                return 1.00f;
    }
  }

  inline int damageMultiplier(EnemyType t) {
    switch (t) {
      case EnemyType::Tank:   return 2;   // dano dobrado
      case EnemyType::Ranged: return 1;
      default:                return 1;
    }
  }

  inline int startHp(EnemyType t) {
    switch (t) {
      case EnemyType::Fast:   return HP_FAST;
      case EnemyType::Tank:   return HP_TANK;
      case EnemyType::Ranged: return HP_RANGED;
      default:                return HP_NORMAL;
    }
  }

  // Distância de ataque ranged
  static constexpr float RANGED_SHOOT_DIST  = 5.5f;
  static constexpr float RANGED_STOP_DIST   = 3.5f;
  static constexpr uint32_t RANGED_COOLDOWN_MS = 1800;

  inline void loadFromSpawn(EnemyState& enemy, const DoomStickCMaps::EnemySpawn& spawn) {
    enemy.x           = spawn.x;
    enemy.y           = spawn.y;
    enemy.alive       = spawn.alive;
    enemy.lastDamageMs = 0;
    enemy.anim        = spawn.anim;
    enemy.type        = static_cast<EnemyType>(spawn.type);
    enemy.hp          = startHp(enemy.type);
    enemy.lastShotMs  = 0;
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

  // Aplica dano ao inimigo; retorna true se morreu
  inline bool hitEnemy(EnemyState& enemy, int dmg = 1) {
    if (!enemy.alive) return false;
    enemy.hp -= dmg;
    if (enemy.hp <= 0) {
      enemy.alive = false;
      return true;
    }
    return false;
  }

  inline bool canRangedShoot(const EnemyState& enemy, uint32_t nowMs) {
    return nowMs - enemy.lastShotMs > RANGED_COOLDOWN_MS;
  }

  inline void markRangedShot(EnemyState& enemy, uint32_t nowMs) {
    enemy.lastShotMs = nowMs;
  }

  inline int countAlive(const EnemyState* enemies, int count) {
    int alive = 0;
    for (int i = 0; i < count; i++) {
      if (enemies[i].alive) alive++;
    }
    return alive;
  }
}
