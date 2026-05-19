#pragma once

#include <Arduino.h>
#include <M5Unified.h>

/*
  DoomStickCWeapons
  v4.0 Full Evolution

  Armas:
  - Pistol  : padrão, sempre disponível
  - Blaster : pickup 'B', tiro de energia
  - Shotgun : pickup 'S', área, mata múltiplos, custo alto

  Controles:
  - B curto  : atirar
  - B longo  : trocar arma (ciclo entre desbloqueadas)
*/

namespace DoomStickCWeapons {

  enum class WeaponId : uint8_t {
    Pistol  = 0,
    Blaster = 1,
    Shotgun = 2
  };

  struct WeaponState {
    WeaponId current;
    bool blasterUnlocked;
    bool shotgunUnlocked;
  };

  static constexpr uint32_t HOLD_TO_SWITCH_MS  = 560;
  static constexpr uint32_t SHORT_PRESS_MAX_MS  = 520;

  inline void reset(WeaponState& state) {
    state.current         = WeaponId::Pistol;
    state.blasterUnlocked = false;
    state.shotgunUnlocked = false;
  }

  inline bool isUnlocked(const WeaponState& state, WeaponId id) {
    if (id == WeaponId::Pistol)  return true;
    if (id == WeaponId::Blaster) return state.blasterUnlocked;
    if (id == WeaponId::Shotgun) return state.shotgunUnlocked;
    return false;
  }

  inline void unlockBlaster(WeaponState& state) {
    state.blasterUnlocked = true;
    state.current = WeaponId::Blaster;
  }

  inline void unlockShotgun(WeaponState& state) {
    state.shotgunUnlocked = true;
    state.current = WeaponId::Shotgun;
  }

  // Cicla entre armas desbloqueadas
  inline void switchNext(WeaponState& state) {
    int cur = static_cast<int>(state.current);
    for (int i = 1; i <= 3; i++) {
      WeaponId next = static_cast<WeaponId>((cur + i) % 3);
      if (isUnlocked(state, next)) {
        state.current = next;
        return;
      }
    }
  }

  inline const char* label(WeaponId id) {
    if (id == WeaponId::Blaster) return "BLASTER";
    if (id == WeaponId::Shotgun) return "SHOTGUN";
    return "PISTOL";
  }

  inline const char* shortLabel(WeaponId id) {
    if (id == WeaponId::Blaster) return "B";
    if (id == WeaponId::Shotgun) return "S";
    return "P";
  }

  inline int ammoCost(WeaponId id) {
    if (id == WeaponId::Blaster) return 2;
    if (id == WeaponId::Shotgun) return 4;
    return 1;
  }

  inline float range(WeaponId id) {
    if (id == WeaponId::Blaster) return 5.5f;
    if (id == WeaponId::Shotgun) return 3.5f;  // curto mas área
    return 8.0f;
  }

  inline float aimCone(WeaponId id) {
    if (id == WeaponId::Blaster) return 0.28f;
    if (id == WeaponId::Shotgun) return 0.55f;  // cone largo — acerta múltiplos
    return 0.16f;
  }

  // Shotgun pode acertar múltiplos inimigos
  inline bool isAreaWeapon(WeaponId id) {
    return id == WeaponId::Shotgun;
  }

  inline int toneBoost(WeaponId id) {
    if (id == WeaponId::Blaster) return 260;
    if (id == WeaponId::Shotgun) return -400;  // som grave
    return 0;
  }

  inline uint32_t flashMs(WeaponId id) {
    if (id == WeaponId::Blaster) return 125;
    if (id == WeaponId::Shotgun) return 160;
    return 95;
  }

  inline uint16_t hudColor(lgfx::LGFX_Device& display, WeaponId id) {
    if (id == WeaponId::Blaster) return display.color565(90, 255, 210);
    if (id == WeaponId::Shotgun) return display.color565(255, 180, 60);
    return display.color565(155, 90, 255);
  }

  inline uint16_t bodyColor(lgfx::LGFX_Device& display, WeaponId id) {
    if (id == WeaponId::Blaster) return display.color565(38, 74, 88);
    if (id == WeaponId::Shotgun) return display.color565(72, 52, 28);
    return display.color565(46, 46, 58);
  }

  inline uint16_t bodyAltColor(lgfx::LGFX_Device& display, WeaponId id) {
    if (id == WeaponId::Blaster) return display.color565(65, 130, 150);
    if (id == WeaponId::Shotgun) return display.color565(120, 88, 42);
    return display.color565(82, 82, 98);
  }

  inline uint16_t edgeColor(lgfx::LGFX_Device& display, WeaponId id) {
    if (id == WeaponId::Blaster) return display.color565(80, 255, 210);
    if (id == WeaponId::Shotgun) return display.color565(255, 200, 80);
    return display.color565(135, 65, 245);
  }

  inline uint16_t flashOuterColor(lgfx::LGFX_Device& display, WeaponId id) {
    if (id == WeaponId::Blaster) return display.color565(80, 255, 230);
    if (id == WeaponId::Shotgun) return display.color565(255, 160, 30);
    return display.color565(255, 225, 30);
  }

  inline uint16_t flashInnerColor(lgfx::LGFX_Device& display, WeaponId id) {
    if (id == WeaponId::Blaster) return display.color565(190, 255, 255);
    if (id == WeaponId::Shotgun) return display.color565(255, 255, 200);
    return display.color565(255, 255, 255);
  }

  inline int barrelHeight(WeaponId id) {
    if (id == WeaponId::Blaster) return 31;
    if (id == WeaponId::Shotgun) return 20;
    return 25;
  }

  inline int barrelWidth(WeaponId id) {
    if (id == WeaponId::Blaster) return 20;
    if (id == WeaponId::Shotgun) return 28;  // cano largo
    return 14;
  }
}
