# DoomStickC — Retro FPS Prototype for M5StickC Plus2

![Status](https://img.shields.io/badge/status-active--prototype-purple)
![Version](https://img.shields.io/badge/version-v4.0--full--evolution-blue)
![Platform](https://img.shields.io/badge/platform-M5StickC%20Plus2-orange)
![Framework](https://img.shields.io/badge/framework-Arduino%20%2B%20M5Unified-green)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

> A lightweight Doom-like / retro FPS prototype built for the **M5StickC Plus2**, using only the device’s built-in buttons, speaker and IMU controls.

---

## ⚠️ Important Notice

**DoomStickC is not an official Doom port.**

This project does **not** include commercial WAD files, copyrighted game assets, original Doom artwork, original Doom levels, or official Doom source/game content.

It is an educational and experimental **Doom-inspired raycasting prototype**, created from scratch for learning, embedded development, maker experimentation, and retro FPS mechanics on tiny hardware.

---

## 🎮 What is DoomStickC?

**DoomStickC** is a small retro FPS experiment designed to run directly on the **M5StickC Plus2**.

The goal is to explore how far we can push a tiny ESP32-based device using:

- Raycasting-style rendering
- Built-in display
- Built-in buttons
- Built-in IMU / accelerometer
- Built-in speaker feedback
- No external controller
- No external hardware
- PlatformIO + Arduino + M5Unified

This project started as a rough proof of concept and is being evolved step by step into a playable mini retro FPS.

---

## 📱 Hardware Target

Tested on:

```text
M5StickC Plus2
```

Main controls use only the built-in device inputs:

| Action | Control |
|---|---|
| Start game | Button A |
| Move forward | Button A |
| Fire | Button B short press |
| Switch weapon | Hold Button B |
| Turn left/right | Tilt device left/right |
| Use / open door / secret wall | Quick press Power button |
| Run | Hold Power button |
| Change difficulty (intro only) | Quick press Power button |
| Skip level start screen | Button A |
| Restart after death/win | Button A + Button B |

---

## ✅ Current Version

```text
DoomStickC MVP v4.0 — Full Evolution
Status: 100% functional on M5StickC Plus2
```

Current validated features:

- Functional boot on M5StickC Plus2
- M5Unified integration
- Tilt calibration
- Internal controls only
- Smooth render using framebuffer / M5Canvas
- PeekSecurity 8-bit loading screen
- Basic raycasting world
- Walls, doors and secret walls
- Minimap with border and color-coded enemy types
- Player HP
- HP bar
- Ammo system
- Shooting
- Weapon switching (cycle through unlocked weapons)
- Pistol weapon profile
- Blaster weapon profile
- Shotgun weapon profile (area weapon, hits multiple enemies)
- Weapon pickup system
- Blaster unlock through pickup `B`
- Shotgun unlock through pickup `S`
- Weapon indicator on HUD
- Weapon-specific HUD accent color
- Weapon-specific crosshair color
- Weapon-specific muzzle flash
- Compact Pistol visual
- Wider/energy-style Blaster visual
- Double-barrel Shotgun visual
- Shooting sound
- Empty ammo sound
- Empty ammo visual border
- 4 enemy types: Normal, Fast, Tank, Ranged
- Normal enemy: red, standard behavior
- Fast enemy: orange, 1.85x speed, smaller sprite
- Tank enemy: green, 3 HP, HP bar visible, double damage
- Ranged enemy: purple, cyclops, stops and shoots from distance
- Enemy hit detection with HP system
- Enemy sprite visual per type
- Simple enemy animation
- Enemy chase behavior
- Ranged enemy projectile with cooldown
- Per-level enemy damage balancing
- Per-level enemy speed balancing
- Difficulty modes: Easy / Normal / Hard
- Difficulty selection on intro screen (Power button cycles)
- Difficulty multipliers for damage, speed and starting ammo
- Score system: kills × 100 + time bonus
- Kill counter on HUD
- Level time displayed on level-clear screen
- Final score, total kills and total time on win screen
- Damage sound
- Red damage flash effect
- Low HP visual pulse
- Health pickup
- Ammo pickup
- Pickup sound
- Complete pickup sprite drawing module
- Door sound
- Secret wall (`W`) opens with Power button
- Secret room discovery feedback
- Exit / level finish pickup
- 5 internal maps (20×16)
- Level progression across 5 levels
- Level indicator on HUD
- Level start preparation screen
- Intermediate level-clear screen with kills and time
- Light HP/ammo bonus between levels
- Win sound
- Death sound
- Start/menu screen
- Button A to start
- Compact retro-style HUD
- Enemy counter
- FPS counter
- Death screen
- Campaign-complete screen with score summary
- Restart with A + B
- Multi-file modular project structure

---

## 🧩 Project Organization

DoomStickC is now split into multiple foundation modules.

Current modules:

```text
include/doomstickc/DoomStickCVersion.h
include/doomstickc/DoomStickCConfig.h
include/doomstickc/DoomStickCBuildInfo.h
include/doomstickc/DoomStickCEnginePlan.h
include/doomstickc/DoomStickCMaps.h
include/doomstickc/DoomStickCPlayer.h
include/doomstickc/DoomStickCEnemies.h
include/doomstickc/DoomStickCUI.h
include/doomstickc/DoomStickCAudio.h
include/doomstickc/DoomStickCRender.h
include/doomstickc/DoomStickCGameplay.h
include/doomstickc/DoomStickCSprites.h
include/doomstickc/DoomStickCWeapons.h

src/doomstickc/DoomStickCVersion.cpp
src/doomstickc/DoomStickCConfig.cpp
src/doomstickc/DoomStickCBuildInfo.cpp
src/doomstickc/DoomStickCEnginePlan.cpp
src/doomstickc/DoomStickCMaps.cpp
src/doomstickc/DoomStickCPlayer.cpp
src/doomstickc/DoomStickCEnemies.cpp
src/doomstickc/DoomStickCUI.cpp
src/doomstickc/DoomStickCAudio.cpp
src/doomstickc/DoomStickCRender.cpp
src/doomstickc/DoomStickCGameplay.cpp
src/doomstickc/DoomStickCSprites.cpp
src/doomstickc/DoomStickCWeapons.cpp
```

### Module Summary

| Module | Purpose |
|---|---|
| `DoomStickCVersion` | Centralizes project/version labels |
| `DoomStickCConfig` | Centralizes hardware/display/audio configuration |
| `DoomStickCBuildInfo` | Stores build/stability labels |
| `DoomStickCEnginePlan` | Documents future internal module split direction |
| `DoomStickCMaps` | Centralizes map data (5 levels, 20×16) and enemy spawn data with type |
| `DoomStickCPlayer` | Centralizes player state, defaults, reset helpers and level bonuses |
| `DoomStickCEnemies` | Centralizes enemy types (Normal/Fast/Tank/Ranged), HP system, AI helpers |
| `DoomStickCUI` | Centralizes UI labels, HUD labels, status messages and loading timing |
| `DoomStickCAudio` | Centralizes speaker setup, audio toggle, volume and tone helpers |
| `DoomStickCRender` | Centralizes safe render helpers such as borders, grid, sky/floor and wall colors |
| `DoomStickCGameplay` | Centralizes difficulty modes, score/time tracking and per-level balance |
| `DoomStickCSprites` | Centralizes pickup/enemy sprite sizing, colors and drawing per enemy type |
| `DoomStickCWeapons` | Centralizes weapon profiles (Pistol/Blaster/Shotgun), unlock state, ammo costs, ranges, aim cones and visual helpers |

---

## 🕹️ Gameplay Controls

| Control | Action |
|---|---|
| Button A | Start / move forward / skip level start screen |
| Button B short press | Shoot |
| Button B hold | Switch weapon (cycles through unlocked weapons) |
| Tilt left/right | Turn camera/player |
| Power quick press | Use / open door / open secret wall |
| Power quick press (intro) | Cycle difficulty: Easy → Normal → Hard |
| Power hold | Temporary run |
| A + B | Restart on final screen |

---

## 🔫 Weapons

### Pistol

Default weapon. Always available.

```text
Ammo cost: 1
Range: longer
Aim cone: precise
Targets: single enemy
HUD: WP
Visual: compact weapon style
```

### Blaster

Unlockable weapon. Collected through pickup `B`.

```text
Ammo cost: 2
Range: shorter
Aim cone: wider
Targets: single enemy
HUD: WB
Visual: wider energy-style weapon
```

### Shotgun

Unlockable weapon. Collected through pickup `S`. Grants +8 ammo on pickup.

```text
Ammo cost: 4
Range: short
Aim cone: very wide (0.55 rad)
Targets: multiple enemies simultaneously
HUD: WS
Visual: double-barrel weapon
```

Weapon behavior:

- Pistol is the default weapon.
- Blaster and Shotgun are locked at the start.
- Pickup `B` unlocks and selects Blaster.
- Pickup `S` unlocks and selects Shotgun.
- Button B short press shoots.
- Button B hold cycles through all unlocked weapons.
- HUD accent color changes based on the selected weapon.
- Crosshair color follows the selected weapon.
- Muzzle flash differs per weapon.
- Shotgun hits all enemies within its wide cone in a single shot.

---

## 👾 Enemy Types

### Normal

Standard enemy. Red sprite.

```text
Speed:  1.0x
HP:     1
Damage: base
Behavior: chases player on sight
```

### Fast

Quick but fragile. Orange sprite, smaller.

```text
Speed:  1.85x
HP:     1
Damage: base
Behavior: chases player faster, harder to dodge
```

### Tank

Slow and tough. Green sprite, larger, HP bar visible.

```text
Speed:  0.52x
HP:     3 (requires 3 hits)
Damage: 2x base
Behavior: slow chase, hits hard, HP bar shown above sprite
```

### Ranged

Keeps distance and shoots. Purple cyclops sprite with antenna.

```text
Speed:  0.60x
HP:     2
Damage: base (projectile)
Behavior: stops at safe distance, fires projectiles with 1.8s cooldown
```

Enemy composition increases in difficulty across the 5 levels.

---

## 🎯 Difficulty Modes

Selected on the intro screen by pressing the Power button. Cycles through:

| Mode | Enemy Damage | Enemy Speed | Starting Ammo |
|---|---|---|---|
| Easy | 60% | 70% | 36 |
| Normal | 100% | 100% | 24 |
| Hard | 155% | 135% | 16 |

The selected difficulty is shown on the intro screen and affects all 5 levels.

---

## 🏆 Score System

Score is calculated at the end of the campaign:

```text
Score = (total kills × 100) + time bonus
Time bonus = max(0, 5000 - total_seconds × 15)
```

- Kill counter is shown live on the HUD during gameplay.
- Level time and level kills are shown on the level-clear screen.
- Final score, total kills and total time are shown on the win screen.

---

## 🗺️ Maps

5 internal maps, each 20×16 tiles.

| Level | Theme | Enemy Focus |
|---|---|---|
| 1 | Entry corridor with secret room | 4 Normal, 2 Fast, 1 Tank, 1 Ranged |
| 2 | Labyrinth with multiple rooms | 2 Normal, 3 Fast, 2 Tank, 1 Ranged |
| 3 | Spiral layout | 2 Normal, 2 Fast, 2 Tank, 2 Ranged |
| 4 | Open arena | 1 Normal, 2 Fast, 3 Tank, 2 Ranged |
| 5 | Final fortress | 0 Normal, 2 Fast, 3 Tank, 3 Ranged |

### Map Cell Legend

| Cell | Meaning |
|---|---|
| `#` | Wall |
| `.` | Floor |
| `D` | Door (opens with Power button) |
| `W` | Secret wall (looks like a wall, opens with Power button) |
| `H` | Health pickup (+30 HP) |
| `M` | Ammo pickup (+12 ammo) |
| `B` | Blaster pickup (unlocks Blaster weapon) |
| `S` | Shotgun pickup (unlocks Shotgun weapon, +8 ammo) |
| `E` | Exit / level finish |

---

### v1.3 — Functional Boot

First stable version using **M5Unified**.

Validated:

- Boot works
- Display works
- Buttons work
- Tilt control works
- Power button handled manually
- Basic Doom-like view running on-device

---

### v1.4 — Smooth Render

Focused on improving visual stability.

Added:

- `M5Canvas` framebuffer
- Reduced screen flicker
- FPS counter
- Better HUD
- Simple enemy sprites
- Basic shooting feedback

---

### v1.5 — Gameplay Foundation

Focused on turning the prototype into a basic playable game.

Added:

- HP system
- Ammo system
- Enemy chase
- Enemy damage
- Pickups
- Death state
- Win state
- Restart flow

---

### v1.6 — Visual Polish

Focused on improving the look and feel without breaking the validated gameplay foundation.

Added:

- Start/menu screen
- Button A to start
- Boot visual style
- More compact retro HUD
- HP bar
- Enemy counter
- Improved fake enemy sprites
- Simple enemy animation
- Red damage feedback
- Stronger shooting flash
- Improved fake wall texture
- Minimap border
- More polished death/win screens

---

### v1.7 — Audio and Feedback

Focused on adding sound and stronger player feedback while preserving the stable v1.6 base.

Added:

- Start sound
- Shooting sound
- Empty ammo sound
- Empty ammo visual feedback
- Damage sound
- Pickup sound
- Door sound
- Win sound
- Death sound
- Pickup flash
- Low HP visual pulse
- Better feedback loop for gameplay actions

---

### v1.8 — Map and Level Expansion

Focused on making the prototype feel more like a small campaign.

Added:

- 3 internal maps
- Simple level progression
- Intermediate `FASE OK` screen
- HUD level indicator
- Per-level enemy placement
- Per-level pickup placement
- Light HP/ammo bonus between levels
- Final `CAMPANHA OK` screen

---

### v1.9 — Code Organization Foundation

Focused on starting internal project organization without breaking validated gameplay.

Added:

- First separated metadata/version module
- `DoomStickCVersion`
- Centralized version labels
- Multi-file PlatformIO structure validated
- Foundation for future engine split

---

### v2.0 — Engine Split Foundation

Focused on starting a safer modular engine structure while preserving the validated v1.9 gameplay.

Added:

- `DoomStickCConfig`
- `DoomStickCBuildInfo`
- `DoomStickCEnginePlan`
- Updated `DoomStickCVersion`
- Centralized hardware/display/audio config
- Centralized build/stability labels
- Internal roadmap module for future split
- Multi-file modular foundation validated on hardware

---

### v2.1 — Maps Module

Focused on moving map data and enemy spawn data into a dedicated module.

Added:

- `DoomStickCMaps`
- 3 internal maps centralized
- Enemy spawns centralized
- Level progression preserved
- Main file reduced safely

---

### v2.2 — Player Module

Focused on moving player state and basic player helpers into a dedicated module.

Added:

- `DoomStickCPlayer`
- Centralized player state
- Centralized initial HP and ammo
- Centralized level bonus helpers
- Centralized player reset helpers
- Centralized low-HP helper
- Preserved movement, tilt, running, HP, ammo and level progression

---

### v2.3 — Enemies Module

Focused on moving enemy state and small enemy helpers into a dedicated module.

Added:

- `DoomStickCEnemies`
- Centralized enemy state
- Centralized damage/speed/cooldown constants
- Spawn loading helper
- Alive count helper
- Animation helper
- Damage cooldown helper

---

### v2.4 — PeekSecurity Loading Screen

Focused on adding the approved custom PeekSecurity 8-bit loading screen.

Added:

- Pixel-style loading screen drawn directly with `M5Canvas`
- PeekSecurity cyberpunk purple/green look
- Hexagon terminal `>_` icon
- DOOMSTICKC + PeekSecurity branding
- Pixel loading bar
- Animated boot/status console
- Button A skip after a short delay
- No external image asset required

---

### v2.5 — UI Foundation

Focused on centralizing UI labels and loading timing.

Added:

- `DoomStickCUI`
- Loading screen labels centralized
- Intro screen labels centralized
- Level-clear/final screen labels centralized
- Loading timing centralized

---

### v2.6 — Audio Module

Focused on centralizing audio behavior.

Added:

- `DoomStickCAudio`
- Audio toggle centralized
- Speaker volume centralized
- Tone constants centralized
- `playToneSafe()` helper centralized
- Speaker initialization moved to audio module

---

### v2.7 — Render Foundation

Focused on creating a safe render foundation module.

Added:

- `DoomStickCRender`
- Frame border helper
- Cyber grid helper
- Raycasting preserved in `main.cpp` for safety

---

### v2.8 — Deeper UI Module

Focused on expanding the UI module.

Added:

- Centralized HUD labels
- Centralized status messages
- Centralized level-clear/final texts
- Loading timing preserved in UI module
- Reduced hardcoded strings in `main.cpp`

---

### v2.9 — Raycasting Render Split

Focused on starting a safe split of the raycasting/render code.

Added:

- Sky/floor render helper
- Wall shade calculation helper
- Wall/door color helper
- Helpers moved to `DoomStickCRender`
- Main raycasting loop preserved in `main.cpp` for safety

---

### v3.0 — Gameplay Polish Foundation

Focused on gameplay polish and a key spawn direction fix.

Added:

- `DoomStickCGameplay`
- Gameplay polish/roadmap foundation
- Fixed player starting direction
- `START_ANGLE` changed from `0.0f` to `1.5707963f`
- Player now starts facing a clearer corridor instead of a wall

---

### v3.1 — Sprite Render Split

Focused on beginning safe sprite render organization.

Added:

- `DoomStickCSprites`
- Pickup colors centralized
- Pickup sizing centralized
- Enemy colors centralized
- Enemy sizing centralized
- Enemy bob helper centralized
- Full drawing routines still preserved in `main.cpp` at this stage

---

### v3.2 — Difficulty and Balance

Focused on adding light per-level balance.

Added:

- Enemy damage by level:
  - L1: 6
  - L2: 7
  - L3: 8
- Enemy speed by level:
  - L1: 0.38
  - L2: 0.42
  - L3: 0.47
- Level bonus balance:
  - L1 -> L2: +20 HP / +10 ammo
  - L2 -> L3: +14 HP / +7 ammo
- Dynamic damage status message
- Custom level bonus helper in `DoomStickCPlayer`

---

### v3.3 — Level Start Polish

Focused on making each level start feel more polished.

Added:

- `GAME_LEVEL_START`
- Short LEVEL 1 / LEVEL 2 / LEVEL 3 preparation screen
- Objective text: find `E`
- Small preparation delay before enemies update
- Button A skip for level-start screen

---

### v3.4 — Full Sprite Drawing Extraction

Focused on extracting complete pickup/enemy sprite drawing into `DoomStickCSprites`.

Added:

- Complete pickup sprite drawing centralized
- Complete enemy sprite drawing centralized
- `main.cpp` reduced safely
- AI, collision, hit detection and gameplay preserved

---

### v3.5 — Weapons Foundation

Focused on adding the first weapon system foundation.

Added:

- `DoomStickCWeapons`
- Pistol weapon profile
- Blaster weapon profile
- B short press to shoot
- B hold to switch weapon
- Weapon HUD indicator
- Pistol ammo cost/range/aim profile
- Blaster ammo cost/range/aim profile
- Weapon-specific tone/flash behavior foundation

---

### v3.6 — Weapon Pickups

Focused on adding weapon pickup progression.

Added:

- Pickup cell `B`
- Blaster unlock through pickup `B`
- Pistol remains default
- Pickup `B` appears in minimap
- Pickup `B` appears in 3D
- Blaster becomes selected after pickup
- B hold switches weapon only after Blaster unlock
- HUD keeps WP/WB behavior

---

### v3.7 — Weapon Visual Polish

Focused on making Pistol and Blaster visually distinct.

Added:

- Weapon-specific HUD accent color
- Weapon-specific crosshair color
- Compact Pistol visual
- Wider/energy-style Blaster visual
- Different muzzle flash per weapon
- Weapon visual helpers in `DoomStickCWeapons`
- Gameplay, ammo cost, pickups, controls and progression preserved

---

### v4.0 — Full Evolution

Focused on a major gameplay expansion across all dimensions.

Added:

- 4 enemy types: Normal, Fast, Tank, Ranged
  - Normal: red, standard behavior
  - Fast: orange, 1.85x speed, smaller sprite
  - Tank: green, 3 HP, HP bar visible above sprite, double damage
  - Ranged: purple cyclops with antenna, stops and fires projectiles with cooldown
- 8 enemies per level (was 4), with increasing type composition across levels
- Shotgun weapon (pickup `S`)
  - Double-barrel visual
  - Wide cone (0.55 rad) — hits multiple enemies simultaneously
  - Ammo cost: 4, short range
  - Grants +8 ammo on pickup
- Weapon cycle now covers all 3 weapons (Pistol → Blaster → Shotgun)
- Ranged enemy projectile system with 1.8s cooldown
- Tank HP system — requires multiple hits, HP bar shown on sprite
- Score system: kills × 100 + time bonus (max 5000)
- Kill counter on HUD during gameplay
- Level time and level kills on level-clear screen
- Final score, total kills and total time on win screen
- Difficulty modes: Easy / Normal / Hard
  - Selected on intro screen with Power button
  - Affects enemy damage, enemy speed and starting ammo
- 5 levels (was 3), maps expanded to 20×16 (was 16×16)
- Secret walls (`W`) — look like normal walls, open with Power button
- Secret room discovery feedback
- Minimap updated: color-coded dots per enemy type
- Per-level enemy composition designed for increasing challenge
- Level balance extended to 5 levels

---

Recommended setup:

- VS Code
- PlatformIO IDE extension
- USB-C data cable
- M5StickC Plus2

Project stack:

```text
Framework: Arduino
Library: M5Unified
Board: m5stick-c
Platform: espressif32
```

---

## 🚀 How to Build and Upload

Open the project folder in VS Code and run:

```bash
pio run
```

To upload to the M5StickC Plus2:

```bash
pio run -t upload
```

If upload fails, check:

- USB cable supports data
- Correct COM port is detected
- CH9102 / USB serial driver is installed
- Device is powered on
- PlatformIO selected the correct environment
- No other PlatformIO/Serial Monitor process is locking `.pio`

If Windows reports that a `.pio` file is being used by another process, close extra terminals/monitors and run:

```powershell
pio run -t clean
Remove-Item -Recurse -Force .pio
pio run -t upload
```

---

## 🔇 Disable Audio

If speaker feedback causes issues on a specific setup, audio can be disabled through the configuration module:

```cpp
static constexpr bool AUDIO_ENABLED = false;
```

Location:

```text
include/doomstickc/DoomStickCConfig.h
```

By default, audio is enabled:

```cpp
static constexpr bool AUDIO_ENABLED = true;
```

---

## 📁 Project Structure

Current structure:

```text
doomstickc-m5stickc-plus2/
├── platformio.ini
├── README.md
├── src/
│   ├── main.cpp
│   └── doomstickc/
│       ├── DoomStickCVersion.cpp
│       ├── DoomStickCConfig.cpp
│       ├── DoomStickCBuildInfo.cpp
│       ├── DoomStickCEnginePlan.cpp
│       ├── DoomStickCMaps.cpp
│       ├── DoomStickCPlayer.cpp
│       ├── DoomStickCEnemies.cpp
│       ├── DoomStickCUI.cpp
│       ├── DoomStickCAudio.cpp
│       ├── DoomStickCRender.cpp
│       ├── DoomStickCGameplay.cpp
│       ├── DoomStickCSprites.cpp
│       └── DoomStickCWeapons.cpp
├── include/
│   └── doomstickc/
│       ├── DoomStickCVersion.h
│       ├── DoomStickCConfig.h
│       ├── DoomStickCBuildInfo.h
│       ├── DoomStickCEnginePlan.h
│       ├── DoomStickCMaps.h
│       ├── DoomStickCPlayer.h
│       ├── DoomStickCEnemies.h
│       ├── DoomStickCUI.h
│       ├── DoomStickCAudio.h
│       ├── DoomStickCRender.h
│       ├── DoomStickCGameplay.h
│       ├── DoomStickCSprites.h
│       └── DoomStickCWeapons.h
└── data/
```

Recommended future structure:

```text
doomstickc-m5stickc-plus2/
├── platformio.ini
├── README.md
├── LICENSE
├── CHANGELOG.md
├── ROADMAP.md
├── src/
├── include/
├── docs/
│   └── development-notes.md
├── media/
│   ├── screenshots/
│   └── videos/
└── data/
```

---

## 🧭 Roadmap

### v4.1 — Map Selector

Planned improvements:

- Add menu/select flow for choosing a specific level
- Preserve default campaign flow
- Keep controls simple for M5StickC Plus2

---

### v4.2 — Save Best Score

Planned improvements:

- Save best score and best time to NVS/flash
- Show best score on win screen
- Persist across reboots

---

### v4.3 — More Enemy Behaviors

Planned improvements:

- Ranged enemy with visible projectile sprite
- Enemy that patrols instead of waiting
- Enemy that calls others when it spots the player

---

### Future Direction

Possible future improvements:

- Better weapon visuals and animations
- More weapon types
- More enemy types
- Optimized raycasting
- More stable FPS
- More maps
- Boss enemy on final level
- Better sprite system
- More polished campaign flow

---

## 🎯 Project Goals

The main goals of DoomStickC are:

- Explore retro FPS rendering on tiny hardware
- Build a playable embedded game prototype
- Use only the M5StickC Plus2 built-in controls
- Keep the project educational and open
- Document the full evolution from prototype to playable mini game
- Create a fun maker/cyberpunk project for the PeekSecurity community
- Keep every version validated on real hardware before moving forward
- Refactor safely, one module at a time
- Preserve stability while expanding gameplay

---

## 🧠 Development Notes

This project is being developed step by step.

The current focus is not perfection.  
The current focus is:

```text
make it run
make it playable
make it smoother
make it fun
organize it safely
split the engine carefully
add gameplay depth
then make it bigger
```

Each version improves one major area at a time.

Validated versions are treated as stable checkpoints.

---

## 👨‍💻 Author

Developed by **PeekSecurity**.

```text
Fala rapaziada, suavidade?
Eu sou o Peek.
```

GitHub:

```text
https://github.com/PSecurity
```

---

## 🟣 PeekSecurity Style

DoomStickC follows the PeekSecurity maker/cyberpunk spirit:

- Embedded hacking
- Tiny devices
- Security culture
- Retro tech
- Purple neon aesthetic
- DIY experimentation
- Learning by building
- Game dev experimentation on unusual hardware

---

## 📜 License

This project is released for educational and experimental purposes.

Recommended license:

```text
MIT License
```

Add a `LICENSE` file before publishing a stable release.

---

## ⭐ Support the Project

If you like the idea, follow the evolution.

This project will keep improving version by version.

```text
DoomStickC is alive.
Tiny hardware. Big experiment.
v4.0 — Full Evolution.
```
