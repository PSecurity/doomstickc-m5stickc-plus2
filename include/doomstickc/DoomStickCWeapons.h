#pragma once

#include <Arduino.h>
#include <M5Unified.h>

/*
  DoomStickCWeapons
  v3.7 Weapon Visual Polish

  Adds safe weapon profiles, unlock state and visual helpers.

  Controls:
  - Short B release: shoot
  - Hold B: switch weapon if unlocked

  Current weapons:
  - Pistol: default, always unlocked
  - Blaster: unlocked by pickup cell 'B'
*/

namespace DoomStickCWeapons {
  enum class WeaponId : uint8_t {
    Pistol = 0,
    Blaster = 1
  };

  struct WeaponState {
    WeaponId current;
    bool blasterUnlocked;
  };

  static constexpr uint32_t HOLD_TO_SWITCH_MS = 560;
  static constexpr uint32_t SHORT_PRESS_MAX_MS = 520;

  inline void reset(WeaponState& state) {
    state.current = WeaponId::Pistol;
    state.blasterUnlocked = false;
  }

  inline bool isUnlocked(const WeaponState& state, WeaponId id) {
    if (id == WeaponId::Pistol) return true;
    return state.blasterUnlocked;
  }

  inline void unlockBlaster(WeaponState& state) {
    state.blasterUnlocked = true;
    state.current = WeaponId::Blaster;
  }

  inline void switchNext(WeaponState& state) {
    if (!state.blasterUnlocked) {
      state.current = WeaponId::Pistol;
      return;
    }

    state.current = (state.current == WeaponId::Pistol) ? WeaponId::Blaster : WeaponId::Pistol;
  }

  inline const char* label(WeaponId id) {
    return (id == WeaponId::Pistol) ? "PISTOL" : "BLASTER";
  }

  inline const char* shortLabel(WeaponId id) {
    return (id == WeaponId::Pistol) ? "P" : "B";
  }

  inline int ammoCost(WeaponId id) {
    return (id == WeaponId::Pistol) ? 1 : 2;
  }

  inline float range(WeaponId id) {
    return (id == WeaponId::Pistol) ? 8.0f : 5.5f;
  }

  inline float aimCone(WeaponId id) {
    return (id == WeaponId::Pistol) ? 0.16f : 0.28f;
  }

  inline int toneBoost(WeaponId id) {
    return (id == WeaponId::Pistol) ? 0 : 260;
  }

  inline uint32_t flashMs(WeaponId id) {
    return (id == WeaponId::Pistol) ? 95 : 125;
  }

  inline uint16_t hudColor(lgfx::LGFX_Device& display, WeaponId id) {
    return (id == WeaponId::Pistol)
      ? display.color565(155, 90, 255)
      : display.color565(90, 255, 210);
  }

  inline uint16_t bodyColor(lgfx::LGFX_Device& display, WeaponId id) {
    return (id == WeaponId::Pistol)
      ? display.color565(46, 46, 58)
      : display.color565(38, 74, 88);
  }

  inline uint16_t bodyAltColor(lgfx::LGFX_Device& display, WeaponId id) {
    return (id == WeaponId::Pistol)
      ? display.color565(82, 82, 98)
      : display.color565(65, 130, 150);
  }

  inline uint16_t edgeColor(lgfx::LGFX_Device& display, WeaponId id) {
    return (id == WeaponId::Pistol)
      ? display.color565(135, 65, 245)
      : display.color565(80, 255, 210);
  }

  inline uint16_t flashOuterColor(lgfx::LGFX_Device& display, WeaponId id) {
    return (id == WeaponId::Pistol)
      ? display.color565(255, 225, 30)
      : display.color565(80, 255, 230);
  }

  inline uint16_t flashInnerColor(lgfx::LGFX_Device& display, WeaponId id) {
    return (id == WeaponId::Pistol)
      ? display.color565(255, 255, 255)
      : display.color565(190, 255, 255);
  }

  inline int barrelHeight(WeaponId id) {
    return (id == WeaponId::Pistol) ? 25 : 31;
  }

  inline int barrelWidth(WeaponId id) {
    return (id == WeaponId::Pistol) ? 14 : 20;
  }
}
