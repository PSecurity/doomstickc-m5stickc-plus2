/*
  DoomStickC MVP v3.7 - Weapon Visual Polish
  Hardware: M5StickC Plus2
  Framework: Arduino + M5Unified

  Base:
    - v3.6 Weapon Pickups validada como funcional.

  Objetivo da v3.7:
    - Diferenciar visualmente Pistol e Blaster sem alterar gameplay.
    - Adicionar desenho/flash/HUD mais claro por arma.
    - Preservar pickups de arma, controles, dificuldade, render, loading PeekSecurity, UI, áudio e framebuffer da v3.6.

  Controles:
    - Tela inicial: Botão A inicia
    - Botão A: andar para frente
    - Botão B: atirar
    - Inclinar esquerda/direita: virar
    - Power click rápido: usar / abrir porta
    - Power segurado: correr temporariamente
    - A + B na tela de morte/vitória: reiniciar
*/

#include <Arduino.h>
#include <M5Unified.h>
#include <math.h>
#include "doomstickc/DoomStickCVersion.h"
#include "doomstickc/DoomStickCConfig.h"
#include "doomstickc/DoomStickCBuildInfo.h"
#include "doomstickc/DoomStickCEnginePlan.h"
#include "doomstickc/DoomStickCMaps.h"
#include "doomstickc/DoomStickCPlayer.h"
#include "doomstickc/DoomStickCEnemies.h"
#include "doomstickc/DoomStickCUI.h"
#include "doomstickc/DoomStickCAudio.h"
#include "doomstickc/DoomStickCRender.h"
#include "doomstickc/DoomStickCGameplay.h"
#include "doomstickc/DoomStickCSprites.h"
#include "doomstickc/DoomStickCWeapons.h"

// v3.7 adds Weapon Visual Polish.
// Gameplay remains intentionally preserved from v3.6.
//
// Existing modules:
//   DoomStickCVersion
//   DoomStickCConfig
//   DoomStickCBuildInfo
//   DoomStickCEnginePlan
//   DoomStickCMaps
//   DoomStickCPlayer
//   DoomStickCEnemies
//   DoomStickCUI
//   DoomStickCAudio
//   DoomStickCRender
//   DoomStickCGameplay
//   DoomStickCSprites
//   DoomStickCWeapons
//
// New v3.7 work:
//   weapon-specific hand sprite, muzzle flash and HUD accent.
//
// Future refactor targets:
//   difficulty modes, objective/score polish, map selector.
//
// -----------------------------
// HARDWARE / TELA
// -----------------------------
static constexpr int SCREEN_W = DoomStickCConfig::SCREEN_W;
static constexpr int SCREEN_H = DoomStickCConfig::SCREEN_H;

static constexpr int PIN_POWER_BUTTON = DoomStickCConfig::PIN_POWER_BUTTON;
static constexpr int PIN_HOLD = DoomStickCConfig::PIN_HOLD;
static constexpr bool POWER_ACTIVE_LOW = DoomStickCConfig::POWER_ACTIVE_LOW;

M5Canvas frame(&M5.Display);

// -----------------------------
// JOGO / RENDER
// -----------------------------
static constexpr float PI_F = 3.14159265f;
static constexpr float FOV = PI_F / 3.0f;
static constexpr float MAX_DEPTH = 14.0f;
static constexpr float RAY_STEP = 0.045f;
static constexpr int RAY_COLUMN_STEP = 2;

static constexpr float WALK_SPEED = 1.55f;
static constexpr float RUN_SPEED  = 2.65f;
static constexpr float TURN_SPEED = 2.15f;

// Tilt / acelerômetro.
static constexpr int TILT_AXIS = 1;
static constexpr bool TILT_INVERT = false;
static constexpr float TILT_DEADZONE = 0.13f;
static constexpr float TILT_GAIN = 1.55f;

// Botão Power manual.
static constexpr uint32_t POWER_CLICK_MAX_MS = 450;
static constexpr uint32_t POWER_HOLD_MS = 650;

// Gameplay
static constexpr int PLAYER_MAX_HP = DoomStickCPlayer::PLAYER_MAX_HP;
static constexpr int PLAYER_START_AMMO = DoomStickCPlayer::PLAYER_START_AMMO;
static constexpr int ENEMY_DAMAGE = DoomStickCEnemies::ENEMY_DAMAGE;
static constexpr uint32_t ENEMY_DAMAGE_COOLDOWN_MS = DoomStickCEnemies::ENEMY_DAMAGE_COOLDOWN_MS;
static constexpr float ENEMY_SPEED = DoomStickCEnemies::ENEMY_SPEED;
static constexpr float ENEMY_ATTACK_DISTANCE = DoomStickCEnemies::ENEMY_ATTACK_DISTANCE;

// Visual
static constexpr uint32_t DAMAGE_FLASH_MS = 170;
static constexpr uint32_t INTRO_MIN_MS = 900;
static constexpr uint32_t LEVEL_CLEAR_PAUSE_MS = 900;
static constexpr uint32_t LOADING_SCREEN_MS = DoomStickCUI::LOADING_SCREEN_MS;
static constexpr uint32_t LOADING_MIN_SKIP_MS = DoomStickCUI::LOADING_MIN_SKIP_MS;

// Audio / feedback.
static constexpr bool AUDIO_ENABLED = DoomStickCAudio::AUDIO_ENABLED;
static constexpr uint8_t AUDIO_VOLUME = DoomStickCAudio::AUDIO_VOLUME;

// -----------------------------
// MAPAS INTERNOS
// # = parede
// . = chão
// D = porta
// H = pickup vida
// M = pickup munição
// E = saída/fim da fase
// -----------------------------
static constexpr int MAP_W = DoomStickCMaps::MAP_W;
static constexpr int MAP_H = DoomStickCMaps::MAP_H;
static constexpr int LEVEL_COUNT = DoomStickCMaps::LEVEL_COUNT;
static constexpr int ENEMY_COUNT = DoomStickCMaps::ENEMY_COUNT;

char worldMap[MAP_H][MAP_W + 1];

using Player = DoomStickCPlayer::PlayerState;
Player player;

using Enemy = DoomStickCEnemies::EnemyState;
Enemy enemies[ENEMY_COUNT];

enum GameState {
  GAME_LOADING,
  GAME_INTRO,
  GAME_LEVEL_START,
  GAME_PLAYING,
  GAME_DEAD,
  GAME_LEVEL_CLEAR,
  GAME_WIN
};

GameState gameState = GAME_INTRO;

int currentLevel = 0;
uint32_t levelClearStartedMs = 0;
uint32_t levelStartStartedMs = 0;

float tiltCenter = 0.0f;
bool imuReady = false;

uint32_t bootMs = 0;
uint32_t loadingStartedMs = 0;
uint32_t lastFrameMs = 0;
uint32_t sprintUntilMs = 0;
uint32_t shootFlashUntilMs = 0;
uint32_t statusUntilMs = 0;
uint32_t damageFlashUntilMs = 0;
uint32_t emptyAmmoFlashUntilMs = 0;
uint32_t pickupFlashUntilMs = 0;
String statusLine = DoomStickCVersion::STATUS_LABEL;

DoomStickCWeapons::WeaponState weaponState;
bool fireButtonWasDown = false;
bool fireButtonHoldFired = false;
uint32_t fireButtonDownAtMs = 0;

bool powerWasDown = false;
bool powerHoldFired = false;
uint32_t powerDownAtMs = 0;

uint32_t fpsLastMs = 0;
int fpsFrames = 0;
int fpsValue = 0;

// -----------------------------
// HELPERS
// -----------------------------
static inline uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return M5.Display.color565(r, g, b);
}

static inline void setStatus(const String& msg, uint32_t ttlMs = 1200) {
  statusLine = msg;
  statusUntilMs = millis() + ttlMs;
}

static inline void playToneSafe(int freq, int durationMs) {
  DoomStickCAudio::playToneSafe(freq, durationMs);
}

static inline bool isPowerDown() {
  int raw = digitalRead(PIN_POWER_BUTTON);
  return POWER_ACTIVE_LOW ? (raw == LOW) : (raw == HIGH);
}

static inline char cellAt(int mx, int my) {
  if (mx < 0 || mx >= MAP_W || my < 0 || my >= MAP_H) return '#';
  return worldMap[my][mx];
}

static inline bool isBlockingCell(int mx, int my) {
  char c = cellAt(mx, my);
  return c == '#' || c == 'D';
}

static inline bool isPickupCell(char c) {
  return c == 'H' || c == 'M' || c == 'E' || c == 'B';
}

static inline float angleNormalize(float a) {
  while (a < -PI_F) a += 2.0f * PI_F;
  while (a >  PI_F) a -= 2.0f * PI_F;
  return a;
}

static int enemiesAliveCount() {
  return DoomStickCEnemies::countAlive(enemies, ENEMY_COUNT);
}

static void copyLevelMap(int level) {
  if (level < 0) level = 0;
  if (level >= LEVEL_COUNT) level = LEVEL_COUNT - 1;

  for (int y = 0; y < MAP_H; y++) {
    for (int x = 0; x <= MAP_W; x++) {
      worldMap[y][x] = DoomStickCMaps::LEVEL_MAPS[level][y][x];
    }
  }
}

static void loadLevel(int level, bool keepStats) {
  if (level < 0) level = 0;
  if (level >= LEVEL_COUNT) level = LEVEL_COUNT - 1;

  currentLevel = level;
  copyLevelMap(currentLevel);

  DoomStickCPlayer::resetPosition(player);

  if (!keepStats) {
    DoomStickCPlayer::resetStats(player);
  } else {
    DoomStickCPlayer::applyLevelBonus(player, DoomStickCGameplay::hpBonusForNextLevel(currentLevel - 1), DoomStickCGameplay::ammoBonusForNextLevel(currentLevel - 1));
  }

  for (int i = 0; i < ENEMY_COUNT; i++) {
    DoomStickCEnemies::loadFromSpawn(enemies[i], DoomStickCMaps::LEVEL_ENEMIES[currentLevel][i]);
  }

  sprintUntilMs = 0;
  shootFlashUntilMs = 0;
  damageFlashUntilMs = 0;
  emptyAmmoFlashUntilMs = 0;
  pickupFlashUntilMs = 0;
  powerWasDown = false;
  powerHoldFired = false;
  fireButtonWasDown = false;
  fireButtonHoldFired = false;

  gameState = GAME_LEVEL_START;
  levelStartStartedMs = millis();
  setStatus(String(DoomStickCUI::STATUS_LEVEL_PREFIX) + String(currentLevel + 1), 1200);
  playToneSafe(DoomStickCAudio::TONE_LEVEL, 120);
}

static void resetGame(bool startPlaying) {
  currentLevel = 0;
  DoomStickCPlayer::resetStats(player);
  DoomStickCWeapons::reset(weaponState);
  copyLevelMap(0);

  DoomStickCPlayer::resetPosition(player);

  for (int i = 0; i < ENEMY_COUNT; i++) {
    DoomStickCEnemies::loadFromSpawn(enemies[i], DoomStickCMaps::LEVEL_ENEMIES[0][i]);
  }

  gameState = startPlaying ? GAME_LEVEL_START : GAME_INTRO;
  if (startPlaying) {
    levelStartStartedMs = millis();
  }
  sprintUntilMs = 0;
  shootFlashUntilMs = 0;
  damageFlashUntilMs = 0;
  emptyAmmoFlashUntilMs = 0;
  pickupFlashUntilMs = 0;
  powerWasDown = false;
  powerHoldFired = false;
  fireButtonWasDown = false;
  fireButtonHoldFired = false;
  setStatus(startPlaying ? (String(DoomStickCUI::STATUS_LEVEL_PREFIX) + String(1)) : String(DoomStickCUI::STATUS_READY), 1000);
}

static bool hasLineOfSight(float x0, float y0, float x1, float y1) {
  float dx = x1 - x0;
  float dy = y1 - y0;
  float dist = sqrtf(dx * dx + dy * dy);
  if (dist < 0.01f) return true;

  float step = 0.08f;
  int steps = (int)(dist / step);
  for (int i = 0; i <= steps; i++) {
    float t = (float)i / (float)max(1, steps);
    int mx = (int)(x0 + dx * t);
    int my = (int)(y0 + dy * t);
    if (isBlockingCell(mx, my)) return false;
  }
  return true;
}

static bool canEnemyMoveTo(float nx, float ny) {
  const float radius = 0.18f;

  if (isBlockingCell((int)(nx + radius), (int)ny)) return false;
  if (isBlockingCell((int)(nx - radius), (int)ny)) return false;
  if (isBlockingCell((int)nx, (int)(ny + radius))) return false;
  if (isBlockingCell((int)nx, (int)(ny - radius))) return false;

  float dx = nx - player.x;
  float dy = ny - player.y;
  if (sqrtf(dx * dx + dy * dy) < 0.32f) return false;

  return true;
}

static void tryMove(float nx, float ny) {
  const float radius = 0.16f;

  if (!isBlockingCell((int)(nx + radius), (int)player.y) &&
      !isBlockingCell((int)(nx - radius), (int)player.y)) {
    player.x = nx;
  }

  if (!isBlockingCell((int)player.x, (int)(ny + radius)) &&
      !isBlockingCell((int)player.x, (int)(ny - radius))) {
    player.y = ny;
  }
}

static void startLevelClear() {
  if (currentLevel >= LEVEL_COUNT - 1) {
    gameState = GAME_WIN;
    playToneSafe(DoomStickCAudio::TONE_WIN, 180);
    setStatus(DoomStickCUI::STATUS_CAMPAIGN_DONE, 3000);
    return;
  }

  gameState = GAME_LEVEL_CLEAR;
  levelClearStartedMs = millis();
  playToneSafe(DoomStickCAudio::TONE_WIN, 120);
  setStatus(DoomStickCUI::STATUS_LEVEL_DONE, 1500);
}

static void handlePickupAtPlayer() {
  int mx = (int)player.x;
  int my = (int)player.y;
  char c = cellAt(mx, my);

  if (c == 'H') {
    player.hp += 30;
    if (player.hp > PLAYER_MAX_HP) player.hp = PLAYER_MAX_HP;
    worldMap[my][mx] = '.';
    pickupFlashUntilMs = millis() + 180;
    playToneSafe(DoomStickCAudio::TONE_PICKUP, 70);
    setStatus(DoomStickCUI::STATUS_HEALTH_PICKUP);
  } else if (c == 'M') {
    player.ammo += 12;
    worldMap[my][mx] = '.';
    pickupFlashUntilMs = millis() + 180;
    playToneSafe(DoomStickCAudio::TONE_PICKUP + 260, 70);
    setStatus(DoomStickCUI::STATUS_AMMO_PICKUP);
  } else if (c == 'B') {
    DoomStickCWeapons::unlockBlaster(weaponState);
    worldMap[my][mx] = '.';
    pickupFlashUntilMs = millis() + 220;
    playToneSafe(DoomStickCAudio::TONE_LEVEL + DoomStickCWeapons::toneBoost(DoomStickCWeapons::WeaponId::Blaster), 100);
    setStatus(DoomStickCUI::STATUS_WEAPON_PICKUP);
  } else if (c == 'E') {
    worldMap[my][mx] = '.';
    startLevelClear();
  }
}

// -----------------------------
// AÇÕES
// -----------------------------
static void useDoorAhead() {
  int tx = (int)(player.x + cosf(player.a) * 0.95f);
  int ty = (int)(player.y + sinf(player.a) * 0.95f);

  if (tx >= 0 && tx < MAP_W && ty >= 0 && ty < MAP_H && worldMap[ty][tx] == 'D') {
    worldMap[ty][tx] = '.';
    playToneSafe(DoomStickCAudio::TONE_DOOR, 80);
    setStatus(DoomStickCUI::STATUS_DOOR_OPEN);
    return;
  }

  playToneSafe(DoomStickCAudio::TONE_EMPTY, 45);
  setStatus(DoomStickCUI::STATUS_NOTHING_TO_USE);
}

static void shoot();

static void switchWeapon() {
  if (!weaponState.blasterUnlocked) {
    playToneSafe(DoomStickCAudio::TONE_EMPTY, 55);
    setStatus(String(DoomStickCUI::STATUS_WEAPON_PREFIX) + String(DoomStickCWeapons::label(weaponState.current)), 900);
    return;
  }

  DoomStickCWeapons::switchNext(weaponState);
  playToneSafe(DoomStickCAudio::TONE_LEVEL + DoomStickCWeapons::toneBoost(weaponState.current), 90);
  setStatus(String(DoomStickCUI::STATUS_WEAPON_PREFIX) + String(DoomStickCWeapons::label(weaponState.current)), 1000);
}

static void updateFireButtonManual() {
  uint32_t now = millis();
  bool down = M5.BtnB.isPressed();

  if (down && !fireButtonWasDown) {
    fireButtonWasDown = true;
    fireButtonHoldFired = false;
    fireButtonDownAtMs = now;
  }

  if (down && !fireButtonHoldFired && (now - fireButtonDownAtMs >= DoomStickCWeapons::HOLD_TO_SWITCH_MS)) {
    fireButtonHoldFired = true;
    switchWeapon();
  }

  if (!down && fireButtonWasDown) {
    uint32_t pressMs = now - fireButtonDownAtMs;

    if (!fireButtonHoldFired && pressMs <= DoomStickCWeapons::SHORT_PRESS_MAX_MS) {
      shoot();
    }

    fireButtonWasDown = false;
    fireButtonHoldFired = false;
  }
}


static void shoot() {
  if (gameState != GAME_PLAYING) return;

  int cost = DoomStickCWeapons::ammoCost(weaponState.current);

  if (player.ammo < cost) {
    emptyAmmoFlashUntilMs = millis() + 180;
    playToneSafe(DoomStickCAudio::TONE_EMPTY, 75);
    setStatus(DoomStickCUI::STATUS_EMPTY_AMMO);
    return;
  }

  player.ammo -= cost;
  shootFlashUntilMs = millis() + DoomStickCWeapons::flashMs(weaponState.current);
  playToneSafe(DoomStickCAudio::TONE_SHOOT + DoomStickCWeapons::toneBoost(weaponState.current), 35);

  int bestIndex = -1;
  float bestScore = 9999.0f;

  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (!enemies[i].alive) continue;

    float dx = enemies[i].x - player.x;
    float dy = enemies[i].y - player.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist > DoomStickCWeapons::range(weaponState.current)) continue;

    float targetAngle = atan2f(dy, dx);
    float delta = fabsf(angleNormalize(targetAngle - player.a));

    if (delta < DoomStickCWeapons::aimCone(weaponState.current) && hasLineOfSight(player.x, player.y, enemies[i].x, enemies[i].y)) {
      float score = delta * 10.0f + dist * 0.1f;
      if (score < bestScore) {
        bestScore = score;
        bestIndex = i;
      }
    }
  }

  if (bestIndex >= 0) {
    enemies[bestIndex].alive = false;
    playToneSafe(DoomStickCAudio::TONE_SHOOT + 420 + DoomStickCWeapons::toneBoost(weaponState.current), 55);
    setStatus(DoomStickCUI::STATUS_ENEMY_DOWN);
  } else {
    setStatus(DoomStickCUI::STATUS_SHOT);
  }
}

static void updatePowerButtonManual() {
  uint32_t now = millis();
  bool down = isPowerDown();

  if (down && !powerWasDown) {
    powerWasDown = true;
    powerHoldFired = false;
    powerDownAtMs = now;
  }

  if (down && !powerHoldFired && (now - powerDownAtMs >= POWER_HOLD_MS)) {
    powerHoldFired = true;
    sprintUntilMs = now + 1500;
    setStatus(DoomStickCUI::STATUS_RUN, 900);
  }

  if (!down && powerWasDown) {
    uint32_t pressMs = now - powerDownAtMs;

    if (!powerHoldFired && pressMs <= POWER_CLICK_MAX_MS) {
      useDoorAhead();
    }

    powerWasDown = false;
    powerHoldFired = false;
  }
}

// -----------------------------
// ENEMY AI
// -----------------------------
static void updateEnemies(float dt) {
  uint32_t now = millis();

  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (!enemies[i].alive) continue;

    DoomStickCEnemies::updateAnimation(enemies[i], dt);

    float dx = player.x - enemies[i].x;
    float dy = player.y - enemies[i].y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < ENEMY_ATTACK_DISTANCE) {
      if (DoomStickCEnemies::canDamage(enemies[i], now)) {
        DoomStickCEnemies::markDamage(enemies[i], now);
        player.hp -= DoomStickCGameplay::enemyDamageForLevel(currentLevel);
        if (player.hp < 0) player.hp = 0;

        damageFlashUntilMs = now + DAMAGE_FLASH_MS;
        playToneSafe(DoomStickCAudio::TONE_DAMAGE, 90);
        setStatus(String("-") + String(DoomStickCGameplay::enemyDamageForLevel(currentLevel)) + String(" Vida"), 700);

        if (player.hp <= 0) {
          gameState = GAME_DEAD;
          playToneSafe(DoomStickCAudio::TONE_DEAD, 250);
          setStatus(DoomStickCUI::STATUS_DEAD, 3000);
          return;
        }
      }
      continue;
    }

    if (dist < 7.5f && hasLineOfSight(enemies[i].x, enemies[i].y, player.x, player.y)) {
      float inv = 1.0f / max(0.01f, dist);
      float vx = dx * inv * DoomStickCGameplay::enemySpeedForLevel(currentLevel) * dt;
      float vy = dy * inv * DoomStickCGameplay::enemySpeedForLevel(currentLevel) * dt;

      float nx = enemies[i].x + vx;
      float ny = enemies[i].y + vy;

      if (canEnemyMoveTo(nx, enemies[i].y)) {
        enemies[i].x = nx;
      }

      if (canEnemyMoveTo(enemies[i].x, ny)) {
        enemies[i].y = ny;
      }
    }
  }
}

// -----------------------------
// CALIBRAÇÃO
// -----------------------------
static void drawCalibrationScreen(const char* msg) {
  M5.Display.clear(BLACK);
  M5.Display.setTextDatum(middle_center);
  M5.Display.setTextColor(rgb(180, 80, 255), BLACK);
  M5.Display.drawString(DoomStickCVersion::APP_NAME, SCREEN_W / 2, 25, 4);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.drawString(DoomStickCVersion::CALIBRATION_LABEL, SCREEN_W / 2, 55, 2);
  M5.Display.drawString("Segure reto e parado", SCREEN_W / 2, 82, 2);
  M5.Display.setTextColor(rgb(120, 255, 180), BLACK);
  M5.Display.drawString(msg, SCREEN_W / 2, 106, 2);
}

static void calibrateTiltSafe() {
  drawCalibrationScreen("Calibrando tilt");

  float sum = 0.0f;
  int count = 0;
  uint32_t start = millis();

  while (millis() - start < 1200) {
    M5.update();

    auto imuUpdate = M5.Imu.update();
    if (imuUpdate) {
      auto data = M5.Imu.getImuData();
      sum += (TILT_AXIS == 0) ? data.accel.x : data.accel.y;
      count++;
    }

    delay(15);
  }

  if (count > 0) {
    tiltCenter = sum / count;
    imuReady = true;
    setStatus(DoomStickCUI::STATUS_READY);
  } else {
    tiltCenter = 0.0f;
    imuReady = false;
    setStatus(DoomStickCUI::STATUS_IMU_MISSING);
  }

  M5.Display.clear(BLACK);
}

// -----------------------------
// DESENHO BASE
// -----------------------------
static void drawFrameBorder(uint16_t color) {
  DoomStickCRender::drawFrameBorder(frame, SCREEN_W, SCREEN_H, color, rgb(35, 20, 55));
}

static void drawCyberGrid() {
  DoomStickCRender::drawCyberGrid(frame, SCREEN_W, SCREEN_H, rgb(28, 18, 42));
}


static void drawPixelHexTerminal(int cx, int cy, uint16_t purple, uint16_t green) {
  // Hexagon outline inspired by PeekSecurity icon, simplified for the tiny display.
  int r = 28;
  int x0 = cx - r;
  int x1 = cx + r;
  int y0 = cy - 18;
  int y1 = cy + 18;

  frame.drawLine(cx - r, cy, cx - r / 2, y0, purple);
  frame.drawLine(cx - r / 2, y0, cx + r / 2, y0, purple);
  frame.drawLine(cx + r / 2, y0, cx + r, cy, purple);
  frame.drawLine(cx + r, cy, cx + r / 2, y1, purple);
  frame.drawLine(cx + r / 2, y1, cx - r / 2, y1, purple);
  frame.drawLine(cx - r / 2, y1, cx - r, cy, purple);

  frame.drawLine(cx - r + 4, cy, cx - r / 2 + 3, y0 + 4, purple);
  frame.drawLine(cx - r / 2 + 3, y0 + 4, cx + r / 2 - 3, y0 + 4, purple);
  frame.drawLine(cx + r / 2 - 3, y0 + 4, cx + r - 4, cy, purple);
  frame.drawLine(cx + r - 4, cy, cx + r / 2 - 3, y1 - 4, purple);
  frame.drawLine(cx + r / 2 - 3, y1 - 4, cx - r / 2 + 3, y1 - 4, purple);
  frame.drawLine(cx - r / 2 + 3, y1 - 4, cx - r + 4, cy, purple);

  frame.setTextDatum(middle_center);
  frame.setTextSize(2);
  frame.setTextColor(green, BLACK);
  frame.drawString(">_", cx, cy - 1);
  frame.setTextSize(1);
  frame.setTextDatum(top_left);
}

static void drawPeekSecurityLoadingScreen() {
  uint32_t now = millis();
  uint32_t elapsed = now - loadingStartedMs;
  int progress = (int)map((long)min(elapsed, LOADING_SCREEN_MS), 0, LOADING_SCREEN_MS, 0, 100);
  if (progress < 8) progress = 8;
  if (progress > 100) progress = 100;

  uint16_t purple = rgb(170, 70, 255);
  uint16_t purpleDim = rgb(65, 25, 105);
  uint16_t green = rgb(90, 255, 120);
  uint16_t greenDim = rgb(35, 120, 55);
  uint16_t panel = rgb(8, 6, 18);

  frame.fillScreen(BLACK);

  // CRT/grid background.
  for (int y = 0; y < SCREEN_H; y += 6) {
    frame.drawFastHLine(0, y, SCREEN_W, rgb(5, 5, 14));
  }
  for (int x = 0; x < SCREEN_W; x += 16) {
    frame.drawFastVLine(x, 0, SCREEN_H, rgb(8, 5, 18));
  }

  // Outer boot panel.
  frame.drawRect(3, 3, SCREEN_W - 6, SCREEN_H - 6, purpleDim);
  frame.drawRect(5, 5, SCREEN_W - 10, SCREEN_H - 10, purple);
  frame.fillRect(8, 8, SCREEN_W - 16, 14, panel);
  frame.drawFastHLine(8, 23, SCREEN_W - 16, purpleDim);

  frame.setTextSize(1);
  frame.setTextDatum(top_left);
  frame.setTextColor(purple, panel);
  frame.setCursor(12, 12);
  frame.print(DoomStickCUI::LOADING_TOP_LABEL);

  // Tiny status icons.
  frame.drawRect(196, 11, 21, 8, greenDim);
  frame.fillRect(199, 13, 4, 4, green);
  frame.fillRect(205, 13, 4, 4, green);
  frame.drawCircle(226, 15, 5, purple);
  frame.drawFastHLine(224, 18, 5, purple);

  // Logo and title.
  drawPixelHexTerminal(53, 59, purple, green);

  frame.setTextDatum(middle_center);
  frame.setTextSize(2);
  frame.setTextColor(purple, BLACK);
  frame.drawString("DOOMSTICKC", 151, 45);
  frame.setTextSize(1);
  frame.setTextColor(purple, BLACK);
  frame.drawString(DoomStickCUI::LOADING_BRAND_LEFT, 134, 65);
  frame.setTextColor(green, BLACK);
  frame.drawString(DoomStickCUI::LOADING_BRAND_RIGHT, 166, 65);

  // Loading label.
  frame.setTextDatum(top_left);
  frame.setTextColor(purple, BLACK);
  frame.setCursor(91, 78);
  frame.print(DoomStickCUI::LOADING_TEXT);

  // Pixel loading bar.
  int bx = 91;
  int by = 91;
  int bw = 106;
  int bh = 11;
  frame.drawRect(bx, by, bw, bh, purple);
  int blocks = 14;
  int filled = map(progress, 0, 100, 0, blocks);
  for (int i = 0; i < blocks; i++) {
    int px = bx + 3 + i * 7;
    uint16_t col = (i < filled) ? purple : rgb(24, 12, 38);
    frame.fillRect(px, by + 3, 5, 5, col);
  }
  frame.setTextColor(green, BLACK);
  frame.setCursor(205, 92);
  frame.print(progress);
  frame.print("%");

  // Status console.
  frame.drawRect(45, 111, 150, 15, purpleDim);
  frame.setTextColor(green, BLACK);
  frame.setCursor(51, 115);

  uint8_t step = (elapsed / 650) % 4;
  if (step == 0) {
    frame.print(DoomStickCUI::LOADING_BOOT_0);
  } else if (step == 1) {
    frame.print(DoomStickCUI::LOADING_BOOT_1);
  } else if (step == 2) {
    frame.print(DoomStickCUI::LOADING_BOOT_2);
  } else {
    frame.print(DoomStickCUI::LOADING_BOOT_3);
  }

  frame.setTextColor(green, BLACK);
  frame.setCursor(199, 116);
  frame.print(DoomStickCUI::LOADING_OK);

  frame.pushSprite(0, 0);
}

static void drawIntroScreen() {
  frame.fillScreen(BLACK);
  drawCyberGrid();
  drawFrameBorder(rgb(135, 65, 245));

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(185, 100, 255), BLACK);
  frame.drawString(DoomStickCVersion::TITLE, SCREEN_W / 2, 24, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString(DoomStickCVersion::HARDWARE, SCREEN_W / 2, 50, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString(DoomStickCVersion::INTRO_LABEL, SCREEN_W / 2, 69, 2);

  frame.setTextColor(rgb(200, 200, 255), BLACK);
  frame.drawString(DoomStickCBuildInfo::ENGINE_LABEL, SCREEN_W / 2, 87, 1);

  uint32_t pulse = (millis() / 280) % 2;
  frame.setTextColor(pulse ? YELLOW : rgb(180, 180, 180), BLACK);
  frame.drawString(DoomStickCUI::INTRO_START, SCREEN_W / 2, 104, 2);

  frame.setTextColor(rgb(145, 90, 255), BLACK);
  frame.drawString(DoomStickCUI::INTRO_CONTROLS, SCREEN_W / 2, 124, 1);

  frame.pushSprite(0, 0);
}

static void drawHud() {
  uint16_t hudBg = rgb(8, 8, 14);
  uint16_t neon = rgb(155, 90, 255);

  frame.fillRect(0, 0, SCREEN_W, 18, hudBg);
  frame.drawFastHLine(0, 18, SCREEN_W, neon);

  frame.setTextDatum(top_left);
  frame.setTextSize(1);

  frame.setTextColor(neon, hudBg);
  frame.setCursor(4, 5);
  frame.print(DoomStickCUI::HUD_LEVEL);
  frame.print(currentLevel + 1);

  frame.setCursor(22, 5);
  frame.print(DoomStickCUI::HUD_HP);

  if (player.hp <= 25) {
    frame.setTextColor(RED, hudBg);
  } else if (player.hp <= 55) {
    frame.setTextColor(YELLOW, hudBg);
  } else {
    frame.setTextColor(rgb(120, 255, 180), hudBg);
  }
  frame.setCursor(40, 5);
  frame.print(player.hp);

  int hpBarW = map(player.hp, 0, PLAYER_MAX_HP, 0, 28);
  frame.drawRect(65, 5, 30, 7, rgb(75, 55, 95));
  frame.fillRect(66, 6, hpBarW, 5, player.hp <= 25 ? RED : rgb(80, 255, 130));

  frame.setTextColor(WHITE, hudBg);
  frame.setCursor(100, 5);
  frame.print(DoomStickCUI::HUD_AMMO);
  frame.setTextColor(player.ammo <= 4 ? RED : WHITE, hudBg);
  frame.setCursor(119, 5);
  frame.print(player.ammo);

  frame.setTextColor(rgb(255, 100, 90), hudBg);
  frame.setCursor(148, 5);
  frame.print(DoomStickCUI::HUD_ENEMIES);
  frame.print(enemiesAliveCount());

  frame.setTextColor(DoomStickCWeapons::hudColor(M5.Display, weaponState.current), hudBg);
  frame.setCursor(181, 5);
  frame.print(DoomStickCUI::HUD_WEAPON);
  frame.print(DoomStickCWeapons::shortLabel(weaponState.current));

  if (millis() < sprintUntilMs) {
    frame.setTextColor(YELLOW, hudBg);
    frame.setCursor(198, 5);
    frame.print(DoomStickCUI::HUD_RUN);
  }

  frame.setTextColor(rgb(130, 255, 180), hudBg);
  frame.setCursor(213, 5);
  frame.print(fpsValue);
  frame.print(DoomStickCUI::HUD_FPS);
}

static void drawWeapon() {
  int cx = SCREEN_W / 2;
  int baseY = SCREEN_H - 1;

  uint16_t metal = DoomStickCWeapons::bodyColor(M5.Display, weaponState.current);
  uint16_t metal2 = DoomStickCWeapons::bodyAltColor(M5.Display, weaponState.current);
  uint16_t dark = rgb(18, 18, 26);
  uint16_t edge = DoomStickCWeapons::edgeColor(M5.Display, weaponState.current);

  int barrelW = DoomStickCWeapons::barrelWidth(weaponState.current);
  int barrelH = DoomStickCWeapons::barrelHeight(weaponState.current);

  if (weaponState.current == DoomStickCWeapons::WeaponId::Blaster) {
    frame.fillRoundRect(cx - 34, baseY - 38, 68, 40, 8, dark);
    frame.drawRoundRect(cx - 34, baseY - 38, 68, 40, 8, edge);
    frame.fillRoundRect(cx - 23, baseY - 30, 46, 30, 6, metal);
    frame.drawFastVLine(cx - 13, baseY - 27, 23, rgb(15, 35, 42));
    frame.drawFastVLine(cx + 13, baseY - 27, 23, rgb(15, 35, 42));
    frame.fillRect(cx - barrelW / 2, baseY - 52, barrelW, barrelH, metal2);
    frame.drawRect(cx - barrelW / 2, baseY - 52, barrelW, barrelH, edge);
    frame.fillCircle(cx - 15, baseY - 18, 3, edge);
    frame.fillCircle(cx + 15, baseY - 18, 3, edge);
  } else {
    frame.fillRoundRect(cx - 28, baseY - 35, 56, 37, 7, dark);
    frame.drawRoundRect(cx - 28, baseY - 35, 56, 37, 7, edge);
    frame.fillRoundRect(cx - 16, baseY - 29, 32, 29, 5, metal);
    frame.drawFastVLine(cx - 8, baseY - 26, 22, rgb(25, 25, 34));
    frame.drawFastVLine(cx + 8, baseY - 26, 22, rgb(25, 25, 34));
    frame.fillRect(cx - barrelW / 2, baseY - 48, barrelW, barrelH, metal2);
    frame.fillRect(cx - 5, baseY - 53, 10, 8, rgb(105, 105, 125));
  }

  if (millis() < shootFlashUntilMs) {
    uint16_t flashOuter = DoomStickCWeapons::flashOuterColor(M5.Display, weaponState.current);
    uint16_t flashInner = DoomStickCWeapons::flashInnerColor(M5.Display, weaponState.current);

    if (weaponState.current == DoomStickCWeapons::WeaponId::Blaster) {
      frame.fillTriangle(cx, baseY - 92, cx - 31, baseY - 50, cx + 31, baseY - 50, flashOuter);
      frame.fillTriangle(cx, baseY - 78, cx - 18, baseY - 54, cx + 18, baseY - 54, flashInner);
      frame.drawCircle(cx, baseY - 62, 13, flashOuter);
    } else {
      frame.fillTriangle(cx, baseY - 80, cx - 23, baseY - 49, cx + 23, baseY - 49, flashOuter);
      frame.fillTriangle(cx, baseY - 70, cx - 12, baseY - 52, cx + 12, baseY - 52, flashInner);
    }
  }

  uint16_t cross = DoomStickCWeapons::hudColor(M5.Display, weaponState.current);
  frame.drawFastHLine(cx - 6, SCREEN_H / 2, 12, cross);
  frame.drawFastVLine(cx, SCREEN_H / 2 - 6, 12, cross);
}

static void drawMiniMap() {
  const int scale = 3;
  const int ox = 4;
  const int oy = 22;

  frame.drawRect(ox - 2, oy - 2, MAP_W * scale + 3, MAP_H * scale + 3, rgb(85, 55, 130));

  for (int y = 0; y < MAP_H; y++) {
    for (int x = 0; x < MAP_W; x++) {
      char c = worldMap[y][x];
      uint16_t col = rgb(14, 14, 20);
      if (c == '#') col = rgb(80, 75, 105);
      if (c == 'D') col = rgb(160, 105, 35);
      if (c == 'H') col = rgb(60, 180, 90);
      if (c == 'M') col = rgb(60, 115, 210);
      if (c == 'E') col = rgb(200, 210, 80);
      if (c == 'B') col = rgb(180, 80, 255);
      frame.fillRect(ox + x * scale, oy + y * scale, scale - 1, scale - 1, col);
    }
  }

  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (!enemies[i].alive) continue;
    int ex = ox + (int)(enemies[i].x * scale);
    int ey = oy + (int)(enemies[i].y * scale);
    frame.fillRect(ex - 1, ey - 1, 3, 3, RED);
  }

  int px = ox + (int)(player.x * scale);
  int py = oy + (int)(player.y * scale);
  frame.fillCircle(px, py, 2, rgb(90, 255, 160));
  frame.drawLine(px, py, px + (int)(cosf(player.a) * 5), py + (int)(sinf(player.a) * 5), rgb(90, 255, 160));
}

static void drawPickups3D() {
  for (int y = 0; y < MAP_H; y++) {
    for (int x = 0; x < MAP_W; x++) {
      char c = worldMap[y][x];
      if (!isPickupCell(c)) continue;

      float cx = x + 0.5f;
      float cy = y + 0.5f;
      float dx = cx - player.x;
      float dy = cy - player.y;
      float dist = sqrtf(dx * dx + dy * dy);
      if (dist < 0.2f || dist > 7.0f) continue;
      if (!hasLineOfSight(player.x, player.y, cx, cy)) continue;

      float targetAngle = atan2f(dy, dx);
      float delta = angleNormalize(targetAngle - player.a);
      if (fabsf(delta) > FOV * 0.56f) continue;

      int sx = (int)((0.5f + delta / FOV) * SCREEN_W);
      int size = DoomStickCSprites::pickupSize(dist);
      int sy = SCREEN_H / 2 + (int)(20.0f / dist);

      DoomStickCSprites::drawPickupSprite(frame, M5.Display, sx, sy, size, c);
    }
  }
}

static void drawEnemies() {
  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (!enemies[i].alive) continue;

    float dx = enemies[i].x - player.x;
    float dy = enemies[i].y - player.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 0.2f || dist > 9.0f) continue;
    if (!hasLineOfSight(player.x, player.y, enemies[i].x, enemies[i].y)) continue;

    float targetAngle = atan2f(dy, dx);
    float delta = angleNormalize(targetAngle - player.a);
    if (fabsf(delta) > FOV * 0.56f) continue;

    int sx = (int)((0.5f + delta / FOV) * SCREEN_W);
    int spriteH = DoomStickCSprites::enemySpriteHeight(dist);
    int spriteW = DoomStickCSprites::enemySpriteWidth(spriteH);

    int bob = DoomStickCSprites::enemyBob(enemies[i].anim);
    int sy = SCREEN_H / 2 - spriteH / 2 + 8 + bob;

    DoomStickCSprites::drawEnemySprite(frame, M5.Display, sx, sy, spriteW, spriteH);
  }
}

static void renderWorld() {
  frame.fillScreen(BLACK);

  DoomStickCRender::drawSkyAndFloor(
    frame,
    SCREEN_W,
    SCREEN_H,
    19,
    rgb(20, 20, 40),
    rgb(28, 24, 23),
    rgb(45, 35, 55)
  );

  for (int x = 0; x < SCREEN_W; x += RAY_COLUMN_STEP) {
    float rayAngle = (player.a - FOV / 2.0f) + ((float)x / (float)SCREEN_W) * FOV;

    float distanceToWall = 0.0f;
    bool hitWall = false;
    char hitCell = '#';

    float eyeX = cosf(rayAngle);
    float eyeY = sinf(rayAngle);

    while (!hitWall && distanceToWall < MAX_DEPTH) {
      distanceToWall += RAY_STEP;
      int testX = (int)(player.x + eyeX * distanceToWall);
      int testY = (int)(player.y + eyeY * distanceToWall);

      hitCell = cellAt(testX, testY);
      if (hitCell == '#' || hitCell == 'D') {
        hitWall = true;
      }
    }

    float corrected = distanceToWall * cosf(rayAngle - player.a);
    if (corrected < 0.08f) corrected = 0.08f;

    int ceiling = (int)((SCREEN_H / 2.0f) - SCREEN_H / corrected);
    int floorY  = SCREEN_H - ceiling;

    if (ceiling < 19) ceiling = 19;
    if (floorY >= SCREEN_H) floorY = SCREEN_H - 1;

    uint8_t shade = DoomStickCRender::computeWallShade(corrected);
    uint16_t wallCol = DoomStickCRender::makeWallColor(M5.Display, hitCell, shade, currentLevel);

    frame.fillRect(x, ceiling, RAY_COLUMN_STEP, floorY - ceiling, wallCol);

    int colIndex = x / RAY_COLUMN_STEP;
    if (colIndex % 8 == 0) {
      frame.drawFastVLine(x, ceiling, floorY - ceiling, rgb(10, 10, 18));
    }
    if (colIndex % 13 == 0) {
      int mid = (ceiling + floorY) / 2;
      frame.drawFastHLine(x, mid, RAY_COLUMN_STEP, rgb(190, 120, 255));
    }
  }

  drawPickups3D();
  drawEnemies();
  drawWeapon();
  drawMiniMap();
  drawHud();

  frame.setTextDatum(bottom_left);
  frame.setTextSize(1);
  frame.setTextColor(rgb(150, 255, 190), BLACK);
  frame.fillRect(0, SCREEN_H - 11, 170, 11, BLACK);

  if (millis() > statusUntilMs) {
    statusLine = imuReady ? DoomStickCUI::STATUS_READY : DoomStickCUI::STATUS_IMU_MISSING;
  }

  frame.drawString(statusLine, 4, SCREEN_H - 1);

  if (millis() < pickupFlashUntilMs) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, rgb(120, 255, 180));
  }

  if (millis() < emptyAmmoFlashUntilMs) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, YELLOW);
    frame.drawRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, YELLOW);
  }

  if (DoomStickCPlayer::isLowHp(player) && ((millis() / 260) % 2 == 0)) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, rgb(130, 20, 20));
  }

  if (millis() < damageFlashUntilMs) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, RED);
    frame.drawRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, RED);
    frame.drawRect(2, 2, SCREEN_W - 4, SCREEN_H - 4, RED);
  }
}


static void drawLevelStartScreen() {
  frame.fillScreen(BLACK);
  drawCyberGrid();
  drawFrameBorder(rgb(135, 65, 245));

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(155, 90, 255), BLACK);
  frame.drawString(DoomStickCVersion::STATUS_LABEL, SCREEN_W / 2, 18, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString(String(DoomStickCUI::LEVEL_START_TITLE_PREFIX) + String(currentLevel + 1), SCREEN_W / 2, 49, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString(DoomStickCUI::LEVEL_START_OBJECTIVE, SCREEN_W / 2, 82, 2);

  frame.setTextColor(YELLOW, BLACK);
  frame.drawString(DoomStickCUI::LEVEL_START_HINT, SCREEN_W / 2, 108, 2);

  frame.pushSprite(0, 0);
}

static void drawLevelClearScreen() {
  frame.fillScreen(BLACK);
  drawCyberGrid();
  drawFrameBorder(rgb(120, 255, 180));

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(155, 90, 255), BLACK);
  frame.drawString(DoomStickCVersion::STATUS_LABEL, SCREEN_W / 2, 18, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString(DoomStickCUI::LEVEL_CLEAR_TITLE, SCREEN_W / 2, 50, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString(String(DoomStickCUI::LEVEL_CLEAR_PREFIX) + String(currentLevel + 2), SCREEN_W / 2, 84, 2);

  frame.setTextColor(YELLOW, BLACK);
  frame.drawString(DoomStickCUI::LEVEL_CLEAR_BONUS, SCREEN_W / 2, 110, 2);

  frame.pushSprite(0, 0);
}

static void drawEndScreen(const char* title, const char* subtitle, uint16_t titleColor) {
  frame.fillScreen(BLACK);
  drawCyberGrid();
  drawFrameBorder(titleColor);

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(155, 90, 255), BLACK);
  frame.drawString(DoomStickCVersion::STATUS_LABEL, SCREEN_W / 2, 20, 2);

  frame.setTextColor(titleColor, BLACK);
  frame.drawString(title, SCREEN_W / 2, 56, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString(subtitle, SCREEN_W / 2, 88, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString(DoomStickCUI::RESTART_HINT, SCREEN_W / 2, 113, 2);

  frame.pushSprite(0, 0);
}

static void renderFrame() {
  if (gameState == GAME_LOADING) {
    drawPeekSecurityLoadingScreen();
    return;
  }

  if (gameState == GAME_INTRO) {
    drawIntroScreen();
    return;
  }

  if (gameState == GAME_LEVEL_START) {
    drawLevelStartScreen();
    return;
  }

  if (gameState == GAME_LEVEL_CLEAR) {
    drawLevelClearScreen();
    return;
  }

  if (gameState == GAME_DEAD) {
    drawEndScreen(DoomStickCUI::DEAD_TITLE, DoomStickCUI::DEAD_SUBTITLE, RED);
    return;
  }

  if (gameState == GAME_WIN) {
    drawEndScreen(DoomStickCUI::WIN_TITLE, DoomStickCUI::WIN_SUBTITLE, rgb(120, 255, 180));
    return;
  }

  renderWorld();
  frame.pushSprite(0, 0);
}

// -----------------------------
// INPUT
// -----------------------------
static void handleInput(float dt) {
  M5.update();

  if (gameState == GAME_LOADING) {
    uint32_t elapsed = millis() - loadingStartedMs;
    if (elapsed >= LOADING_SCREEN_MS || (elapsed >= LOADING_MIN_SKIP_MS && M5.BtnA.wasPressed())) {
      gameState = GAME_INTRO;
      bootMs = millis();
    }
    return;
  }

  if (gameState == GAME_INTRO) {
    if (M5.BtnA.wasPressed() && millis() - bootMs > INTRO_MIN_MS) {
      playToneSafe(DoomStickCAudio::TONE_START, 100);
      resetGame(true);
    }
    return;
  }

  if (gameState == GAME_LEVEL_START) {
    if (millis() - levelStartStartedMs >= DoomStickCGameplay::LEVEL_START_PAUSE_MS || M5.BtnA.wasPressed()) {
      gameState = GAME_PLAYING;
      setStatus(String(DoomStickCUI::STATUS_LEVEL_PREFIX) + String(currentLevel + 1), 800);
    }
    return;
  }

  if (gameState == GAME_LEVEL_CLEAR) {
    if (millis() - levelClearStartedMs >= LEVEL_CLEAR_PAUSE_MS) {
      loadLevel(currentLevel + 1, true);
    }
    return;
  }

  if (gameState != GAME_PLAYING) {
    if (M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
      resetGame(true);
    }
    return;
  }

  updateFireButtonManual();

  updatePowerButtonManual();

  float tiltValue = 0.0f;
  auto imuUpdate = M5.Imu.update();

  if (imuUpdate) {
    auto data = M5.Imu.getImuData();
    tiltValue = ((TILT_AXIS == 0) ? data.accel.x : data.accel.y) - tiltCenter;
    imuReady = true;
  }

  if (TILT_INVERT) tiltValue = -tiltValue;

  if (fabsf(tiltValue) > TILT_DEADZONE) {
    float turn = tiltValue * TILT_GAIN * TURN_SPEED * dt;
    player.a += turn;
    player.a = angleNormalize(player.a);
  }

  bool forward = M5.BtnA.isPressed();
  float speed = (millis() < sprintUntilMs) ? RUN_SPEED : WALK_SPEED;

  if (forward) {
    float nx = player.x + cosf(player.a) * speed * dt;
    float ny = player.y + sinf(player.a) * speed * dt;
    tryMove(nx, ny);
    handlePickupAtPlayer();
  }

  updateEnemies(dt);
}

static void updateFpsCounter() {
  fpsFrames++;
  uint32_t now = millis();
  if (now - fpsLastMs >= 1000) {
    fpsValue = fpsFrames;
    fpsFrames = 0;
    fpsLastMs = now;
  }
}

// -----------------------------
// SETUP / LOOP
// -----------------------------
void setup() {
  pinMode(PIN_HOLD, OUTPUT);
  digitalWrite(PIN_HOLD, HIGH);

  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  cfg.clear_display = true;
  cfg.internal_imu = true;
  cfg.internal_spk = AUDIO_ENABLED;
  cfg.internal_mic = false;
  M5.begin(cfg);

  DoomStickCAudio::begin();

  pinMode(PIN_POWER_BUTTON, INPUT);

  M5.Display.setRotation(1);
  M5.Display.setBrightness(115);
  M5.Display.setTextSize(1);
  M5.Display.clear(BLACK);

  frame.setColorDepth(16);
  frame.createSprite(SCREEN_W, SCREEN_H);
  frame.setTextSize(1);
  frame.setTextDatum(top_left);

  resetGame(false);
  calibrateTiltSafe();

  gameState = GAME_LOADING;
  loadingStartedMs = millis();
  bootMs = millis();
  fpsLastMs = millis();
  lastFrameMs = millis();
}

void loop() {
  uint32_t now = millis();
  float dt = (now - lastFrameMs) / 1000.0f;
  lastFrameMs = now;

  if (dt > 0.05f) dt = 0.05f;

  handleInput(dt);
  renderFrame();
  updateFpsCounter();

  delay(6);
}
