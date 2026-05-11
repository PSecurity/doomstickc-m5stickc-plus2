# DoomStickC — Retro FPS Prototype for M5StickC Plus2

![Status](https://img.shields.io/badge/status-active--prototype-purple)
![Version](https://img.shields.io/badge/version-v1.7--audio--feedback-blue)
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
````

Main controls use only the built-in device inputs:

| Action                  | Control                  |
| ----------------------- | ------------------------ |
| Start game              | Button A                 |
| Move forward            | Button A                 |
| Fire                    | Button B                 |
| Turn left/right         | Tilt device left/right   |
| Use / open door         | Quick press Power button |
| Run                     | Hold Power button        |
| Restart after death/win | Button A + Button B      |

---

## ✅ Current Version

```text
DoomStickC MVP v1.7 — Audio and Feedback
Status: 100% functional on M5StickC Plus2
```

Current validated features:

* Functional boot on M5StickC Plus2
* M5Unified integration
* Tilt calibration
* Internal controls only
* Smooth render using framebuffer / M5Canvas
* Basic raycasting world
* Walls and doors
* Minimap with border
* Player HP
* HP bar
* Ammo system
* Shooting
* Stronger shooting flash
* Shooting sound
* Empty ammo sound
* Empty ammo visual border
* Enemy hit detection
* Simple enemies
* Improved fake enemy sprites
* Simple enemy animation
* Enemy chase behavior
* Enemy damage
* Damage sound
* Red damage flash effect
* Low HP visual pulse
* Health pickup
* Ammo pickup
* Pickup sound
* Pickup visual flash
* Door sound
* Exit / level finish pickup
* Win sound
* Death sound
* Start/menu screen
* Button A to start
* Compact retro-style HUD
* Enemy counter
* FPS counter
* Death screen
* Win screen
* Restart with A + B

---

## 🕹️ Gameplay Controls

| Control           | Action                  |
| ----------------- | ----------------------- |
| Button A          | Start / move forward    |
| Button B          | Shoot                   |
| Tilt left/right   | Turn camera/player      |
| Power quick press | Use / open door         |
| Power hold        | Temporary run           |
| A + B             | Restart on final screen |

---

## 🧪 Prototype Versions

### v1.3 — Functional Boot

First stable version using **M5Unified**.

Validated:

* Boot works
* Display works
* Buttons work
* Tilt control works
* Power button handled manually
* Basic Doom-like view running on-device

---

### v1.4 — Smooth Render

Focused on improving visual stability.

Added:

* `M5Canvas` framebuffer
* Reduced screen flicker
* FPS counter
* Better HUD
* Simple enemy sprites
* Basic shooting feedback

---

### v1.5 — Gameplay Foundation

Focused on turning the prototype into a basic playable game.

Added:

* HP system
* Ammo system
* Enemy chase
* Enemy damage
* Pickups
* Death state
* Win state
* Restart flow

---

### v1.6 — Visual Polish

Focused on improving the look and feel without breaking the validated gameplay foundation.

Added:

* Start/menu screen
* Button A to start
* Boot visual style
* More compact retro HUD
* HP bar
* Enemy counter
* Improved fake enemy sprites
* Simple enemy animation
* Red damage feedback
* Stronger shooting flash
* Improved fake wall texture
* Minimap border
* More polished death/win screens

---

### v1.7 — Audio and Feedback

Focused on adding sound and stronger player feedback while preserving the stable v1.6 base.

Added:

* Start sound
* Shooting sound
* Empty ammo sound
* Empty ammo visual feedback
* Damage sound
* Pickup sound
* Door sound
* Win sound
* Death sound
* Pickup flash
* Low HP visual pulse
* Better feedback loop for gameplay actions

---

## 🛠️ Build Requirements

Recommended setup:

* VS Code
* PlatformIO IDE extension
* USB-C data cable
* M5StickC Plus2

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

* USB cable supports data
* Correct COM port is detected
* CH9102 / USB serial driver is installed
* Device is powered on
* PlatformIO selected the correct environment

---

## 🔇 Disable Audio

If speaker feedback causes issues on a specific setup, audio can be disabled in `src/main.cpp`:

```cpp
static constexpr bool AUDIO_ENABLED = false;
```

By default, audio is enabled:

```cpp
static constexpr bool AUDIO_ENABLED = true;
```

---

## 📁 Project Structure

```text
doomstickc-m5stickc-plus2/
├── platformio.ini
├── README.md
├── src/
│   └── main.cpp
├── include/
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
│   └── main.cpp
├── docs/
│   └── development-notes.md
├── media/
│   ├── screenshots/
│   └── videos/
└── data/
```

---

## 🧭 Roadmap

### v1.8 — Map and Level Expansion

Planned improvements:

* Multiple maps
* Better level layout
* More doors
* More pickups
* Enemy placement balancing
* Simple level progression
* Start of a small campaign-like flow

---

### v1.9 — Code Organization

Planned improvements:

* Split rendering, player, map, enemies, audio and UI into separate files
* Cleaner architecture
* Easier maintenance
* Better version control for future features

---

### v2.0 — Engine Refactor

Possible future direction:

* Cleaner engine structure
* Better sprite system
* Optimized raycasting
* More stable FPS
* Asset compression experiments
* More advanced level logic

---

## 🎯 Project Goals

The main goals of DoomStickC are:

* Explore retro FPS rendering on tiny hardware
* Build a playable embedded game prototype
* Use only the M5StickC Plus2 built-in controls
* Keep the project educational and open
* Document the full evolution from prototype to playable mini game
* Create a fun maker/cyberpunk project for the PeekSecurity community

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
then make it beautiful
```

The first versions may look rough, flickery, or experimental. That is expected.

Each version improves one major area at a time.

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

* Embedded hacking
* Tiny devices
* Security culture
* Retro tech
* Purple neon aesthetic
* DIY experimentation
* Learning by building

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
```