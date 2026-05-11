/*
<<<<<<< HEAD
  DoomStickC MVP v1.7 - Audio and Feedback
=======
  DoomStickC MVP v1.8 - Map and Level Expansion
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  Hardware: M5StickC Plus2
  Framework: Arduino + M5Unified

  Base:
<<<<<<< HEAD
    - v1.6 Visual Polish validada como funcional.

  Objetivo da v1.7:
    - Adicionar áudio simples sem quebrar a gameplay funcional.
    - Adicionar feedback sonoro de tiro, dano, pickup, porta e fim de fase.
    - Adicionar feedback visual para munição vazia.
    - Adicionar pulso visual de HP baixo.
    - Manter os controles e a base visual da v1.6.
=======
    - v1.7 Audio and Feedback validada como funcional.

  Objetivo da v1.8:
    - Adicionar progressão simples de fases.
    - Adicionar múltiplos mapas internos.
    - Adicionar contagem de fase no HUD.
    - Reposicionar inimigos por fase.
    - Reposicionar pickups por fase.
    - Preservar gameplay, visual, áudio, controles e framebuffer.
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

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

// -----------------------------
// HARDWARE / TELA
// -----------------------------
static constexpr int SCREEN_W = 240;
static constexpr int SCREEN_H = 135;

static constexpr int PIN_POWER_BUTTON = 35;
static constexpr int PIN_HOLD = 4;
static constexpr bool POWER_ACTIVE_LOW = true;

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
static constexpr int PLAYER_MAX_HP = 100;
static constexpr int PLAYER_START_AMMO = 24;
static constexpr int ENEMY_DAMAGE = 7;
static constexpr uint32_t ENEMY_DAMAGE_COOLDOWN_MS = 850;
static constexpr float ENEMY_SPEED = 0.42f;
static constexpr float ENEMY_ATTACK_DISTANCE = 0.55f;

// Visual
static constexpr uint32_t DAMAGE_FLASH_MS = 170;
static constexpr uint32_t INTRO_MIN_MS = 900;
<<<<<<< HEAD

// Audio / feedback.
// Se o speaker causar qualquer problema no seu ambiente, troque AUDIO_ENABLED para false.
static constexpr bool AUDIO_ENABLED = true;
static constexpr uint8_t AUDIO_VOLUME = 110;

// Frequências simples para feedback estilo protótipo.
=======
static constexpr uint32_t LEVEL_CLEAR_PAUSE_MS = 900;

// Audio / feedback.
static constexpr bool AUDIO_ENABLED = true;
static constexpr uint8_t AUDIO_VOLUME = 110;

>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
static constexpr int TONE_SHOOT = 1800;
static constexpr int TONE_EMPTY = 180;
static constexpr int TONE_DAMAGE = 130;
static constexpr int TONE_PICKUP = 1200;
static constexpr int TONE_DOOR = 780;
static constexpr int TONE_START = 920;
static constexpr int TONE_WIN = 1550;
static constexpr int TONE_DEAD = 90;
<<<<<<< HEAD
=======
static constexpr int TONE_LEVEL = 1320;
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

// -----------------------------
// MAPAS INTERNOS
// # = parede
// . = chão
// D = porta
// H = pickup vida
// M = pickup munição
// E = saída/fim da fase
// -----------------------------
static constexpr int MAP_W = 16;
static constexpr int MAP_H = 16;
static constexpr int LEVEL_COUNT = 3;
static constexpr int ENEMY_COUNT = 4;

const char levelMaps[LEVEL_COUNT][MAP_H][MAP_W + 1] = {
  {
    "################",
    "#..............#",
    "#..####....M...#",
    "#..#..#....D...#",
    "#..#..#........#",
    "#..#..####.....#",
    "#.......H......#",
    "#......##......#",
    "#......##..#####",
    "#..............#",
    "#####..D.......#",
    "#..............#",
    "#....####..M...#",
    "#..............#",
    "#............E.#",
    "################"
  },
  {
    "################",
    "#......M.......#",
    "#..######..##..#",
    "#..#....#..#...#",
    "#..#D##.#..#E..#",
    "#..#....#..##..#",
    "#..####.#......#",
    "#.......#..#####",
    "#####D###......#",
    "#..............#",
    "#..H....####...#",
    "#.......#..#...#",
    "#..##...#..#...#",
    "#..##......M...#",
    "#..............#",
    "################"
  },
  {
    "################",
    "#M.............#",
    "#..##########..#",
    "#..#........#..#",
    "#..#..####..#..#",
    "#..#..#H.#..#..#",
    "#..#..#..#..#..#",
    "#..D..#..D..#..#",
    "#..#..#..#..#..#",
    "#..#..####..#..#",
    "#..#........#..#",
    "#..##########..#",
    "#......M.......#",
    "#..........H...#",
    "#............E.#",
    "################"
  }
};

char worldMap[MAP_H][MAP_W + 1];

struct Player {
  float x;
  float y;
  float a;
  int hp;
  int ammo;
} player;

struct Enemy {
  float x;
  float y;
  bool alive;
  uint32_t lastDamageMs;
  float anim;
};

<<<<<<< HEAD
static constexpr int ENEMY_COUNT = 4;
const Enemy initialEnemies[ENEMY_COUNT] = {
  {10.5f, 3.5f, true, 0, 0.0f},
  {12.5f, 9.5f, true, 0, 1.0f},
  {5.5f, 12.5f, true, 0, 2.0f},
  {10.5f, 13.5f, true, 0, 3.0f}
=======
const Enemy levelEnemies[LEVEL_COUNT][ENEMY_COUNT] = {
  {
    {10.5f, 3.5f, true, 0, 0.0f},
    {12.5f, 9.5f, true, 0, 1.0f},
    {5.5f, 12.5f, true, 0, 2.0f},
    {10.5f, 13.5f, true, 0, 3.0f}
  },
  {
    {12.5f, 2.5f, true, 0, 0.0f},
    {4.5f, 6.5f, true, 0, 1.0f},
    {10.5f, 9.5f, true, 0, 2.0f},
    {6.5f, 13.5f, true, 0, 3.0f}
  },
  {
    {12.5f, 3.5f, true, 0, 0.0f},
    {3.5f, 4.5f, true, 0, 1.0f},
    {12.5f, 10.5f, true, 0, 2.0f},
    {8.5f, 13.5f, true, 0, 3.0f}
  }
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
};

Enemy enemies[ENEMY_COUNT];

enum GameState {
  GAME_INTRO,
  GAME_PLAYING,
  GAME_DEAD,
  GAME_LEVEL_CLEAR,
  GAME_WIN
};

GameState gameState = GAME_INTRO;
<<<<<<< HEAD
=======

int currentLevel = 0;
uint32_t levelClearStartedMs = 0;
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

float tiltCenter = 0.0f;
bool imuReady = false;

uint32_t bootMs = 0;
uint32_t lastFrameMs = 0;
uint32_t sprintUntilMs = 0;
uint32_t shootFlashUntilMs = 0;
uint32_t statusUntilMs = 0;
uint32_t damageFlashUntilMs = 0;
uint32_t emptyAmmoFlashUntilMs = 0;
uint32_t pickupFlashUntilMs = 0;
<<<<<<< HEAD
String statusLine = "DoomStickC v1.7";
=======
String statusLine = "DoomStickC v1.8";
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

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
  if (!AUDIO_ENABLED) return;
  if (freq <= 0 || durationMs <= 0) return;
<<<<<<< HEAD

  // M5Unified usa fila interna para o speaker; tons curtos funcionam bem para feedback.
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  M5.Speaker.tone(freq, durationMs);
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
  return c == 'H' || c == 'M' || c == 'E';
}

static inline float angleNormalize(float a) {
  while (a < -PI_F) a += 2.0f * PI_F;
  while (a >  PI_F) a -= 2.0f * PI_F;
  return a;
}

static int enemiesAliveCount() {
  int n = 0;
  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (enemies[i].alive) n++;
  }
  return n;
}

<<<<<<< HEAD
static void copyInitialMap() {
=======
static void copyLevelMap(int level) {
  if (level < 0) level = 0;
  if (level >= LEVEL_COUNT) level = LEVEL_COUNT - 1;

>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  for (int y = 0; y < MAP_H; y++) {
    for (int x = 0; x <= MAP_W; x++) {
      worldMap[y][x] = levelMaps[level][y][x];
    }
  }
}

<<<<<<< HEAD
static void resetGame(bool startPlaying) {
  copyInitialMap();
=======
static void loadLevel(int level, bool keepStats) {
  if (level < 0) level = 0;
  if (level >= LEVEL_COUNT) level = LEVEL_COUNT - 1;

  currentLevel = level;
  copyLevelMap(currentLevel);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

  player.x = 2.5f;
  player.y = 2.5f;
  player.a = 0.0f;

  if (!keepStats) {
    player.hp = PLAYER_MAX_HP;
    player.ammo = PLAYER_START_AMMO;
  } else {
    player.hp += 18;
    if (player.hp > PLAYER_MAX_HP) player.hp = PLAYER_MAX_HP;
    player.ammo += 8;
  }

  for (int i = 0; i < ENEMY_COUNT; i++) {
    enemies[i] = levelEnemies[currentLevel][i];
  }

<<<<<<< HEAD
  gameState = startPlaying ? GAME_PLAYING : GAME_INTRO;
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  sprintUntilMs = 0;
  shootFlashUntilMs = 0;
  damageFlashUntilMs = 0;
  emptyAmmoFlashUntilMs = 0;
  pickupFlashUntilMs = 0;
  powerWasDown = false;
  powerHoldFired = false;
<<<<<<< HEAD
  setStatus(startPlaying ? "Boa sorte" : "Pronto", 1000);
=======

  gameState = GAME_PLAYING;
  setStatus("Fase " + String(currentLevel + 1), 1200);
  playToneSafe(TONE_LEVEL, 120);
}

static void resetGame(bool startPlaying) {
  currentLevel = 0;
  player.hp = PLAYER_MAX_HP;
  player.ammo = PLAYER_START_AMMO;
  copyLevelMap(0);

  player.x = 2.5f;
  player.y = 2.5f;
  player.a = 0.0f;

  for (int i = 0; i < ENEMY_COUNT; i++) {
    enemies[i] = levelEnemies[0][i];
  }

  gameState = startPlaying ? GAME_PLAYING : GAME_INTRO;
  sprintUntilMs = 0;
  shootFlashUntilMs = 0;
  damageFlashUntilMs = 0;
  emptyAmmoFlashUntilMs = 0;
  pickupFlashUntilMs = 0;
  powerWasDown = false;
  powerHoldFired = false;
  setStatus(startPlaying ? "Fase 1" : "Pronto", 1000);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
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
    playToneSafe(TONE_WIN, 180);
    setStatus("Campanha concluida", 3000);
    return;
  }

  gameState = GAME_LEVEL_CLEAR;
  levelClearStartedMs = millis();
  playToneSafe(TONE_WIN, 120);
  setStatus("Fase concluida", 1500);
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
    playToneSafe(TONE_PICKUP, 70);
    setStatus("+30 Vida");
  } else if (c == 'M') {
    player.ammo += 12;
    worldMap[my][mx] = '.';
    pickupFlashUntilMs = millis() + 180;
    playToneSafe(TONE_PICKUP + 260, 70);
    setStatus("+12 Municao");
  } else if (c == 'E') {
<<<<<<< HEAD
    gameState = GAME_WIN;
    playToneSafe(TONE_WIN, 180);
    setStatus("Fase concluida", 3000);
=======
    worldMap[my][mx] = '.';
    startLevelClear();
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
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
    playToneSafe(TONE_DOOR, 80);
    setStatus("Porta aberta");
    return;
  }

  playToneSafe(TONE_EMPTY, 45);
  setStatus("Nada para usar");
}

static void shoot() {
  if (gameState != GAME_PLAYING) return;

  if (player.ammo <= 0) {
    emptyAmmoFlashUntilMs = millis() + 180;
    playToneSafe(TONE_EMPTY, 75);
    setStatus("Sem municao");
    return;
  }

  player.ammo--;
  shootFlashUntilMs = millis() + 95;
  playToneSafe(TONE_SHOOT, 35);

  int bestIndex = -1;
  float bestScore = 9999.0f;

  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (!enemies[i].alive) continue;

    float dx = enemies[i].x - player.x;
    float dy = enemies[i].y - player.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist > 8.0f) continue;

    float targetAngle = atan2f(dy, dx);
    float delta = fabsf(angleNormalize(targetAngle - player.a));

    if (delta < 0.16f && hasLineOfSight(player.x, player.y, enemies[i].x, enemies[i].y)) {
      float score = delta * 10.0f + dist * 0.1f;
      if (score < bestScore) {
        bestScore = score;
        bestIndex = i;
      }
    }
  }

  if (bestIndex >= 0) {
    enemies[bestIndex].alive = false;
    playToneSafe(TONE_SHOOT + 420, 55);
    setStatus("Inimigo abatido");
  } else {
    setStatus("Tiro");
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
    setStatus("Corrida ativada", 900);
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

    enemies[i].anim += dt * 5.0f;

    float dx = player.x - enemies[i].x;
    float dy = player.y - enemies[i].y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < ENEMY_ATTACK_DISTANCE) {
      if (now - enemies[i].lastDamageMs > ENEMY_DAMAGE_COOLDOWN_MS) {
        enemies[i].lastDamageMs = now;
        player.hp -= ENEMY_DAMAGE;
        if (player.hp < 0) player.hp = 0;

        damageFlashUntilMs = now + DAMAGE_FLASH_MS;
        playToneSafe(TONE_DAMAGE, 90);
        setStatus("-7 Vida", 700);

        if (player.hp <= 0) {
          gameState = GAME_DEAD;
          playToneSafe(TONE_DEAD, 250);
          setStatus("Voce morreu", 3000);
          return;
        }
      }
      continue;
    }

    if (dist < 7.5f && hasLineOfSight(enemies[i].x, enemies[i].y, player.x, player.y)) {
      float inv = 1.0f / max(0.01f, dist);
      float vx = dx * inv * ENEMY_SPEED * dt;
      float vy = dy * inv * ENEMY_SPEED * dt;

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
  M5.Display.drawString("DoomStickC", SCREEN_W / 2, 25, 4);
  M5.Display.setTextColor(WHITE, BLACK);
<<<<<<< HEAD
  M5.Display.drawString("v1.7 Audio Feedback", SCREEN_W / 2, 55, 2);
=======
  M5.Display.drawString("v1.8 Map Expansion", SCREEN_W / 2, 55, 2);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
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
    setStatus("Pronto");
  } else {
    tiltCenter = 0.0f;
    imuReady = false;
    setStatus("IMU sem leitura");
  }

  M5.Display.clear(BLACK);
}

// -----------------------------
// DESENHO BASE
// -----------------------------
static void drawFrameBorder(uint16_t color) {
  frame.drawRect(0, 0, SCREEN_W, SCREEN_H, color);
  frame.drawRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, rgb(35, 20, 55));
}

static void drawCyberGrid() {
  uint16_t grid = rgb(28, 18, 42);
  for (int x = 0; x < SCREEN_W; x += 20) {
    frame.drawFastVLine(x, 0, SCREEN_H, grid);
  }
  for (int y = 0; y < SCREEN_H; y += 15) {
    frame.drawFastHLine(0, y, SCREEN_W, grid);
  }
}

static void drawIntroScreen() {
  frame.fillScreen(BLACK);
  drawCyberGrid();
  drawFrameBorder(rgb(135, 65, 245));

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(185, 100, 255), BLACK);
<<<<<<< HEAD
  frame.drawString("DOOMSTICKC", SCREEN_W / 2, 25, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString("M5StickC Plus2", SCREEN_W / 2, 52, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString("v1.7 Audio Feedback", SCREEN_W / 2, 72, 2);

  uint32_t pulse = (millis() / 280) % 2;
  frame.setTextColor(pulse ? YELLOW : rgb(180, 180, 180), BLACK);
  frame.drawString("Pressione A para iniciar", SCREEN_W / 2, 100, 2);

  frame.setTextColor(rgb(145, 90, 255), BLACK);
  frame.drawString("A=MOVER  B=TIRO  TILT=VIRAR", SCREEN_W / 2, 123, 1);
=======
  frame.drawString("DOOMSTICKC", SCREEN_W / 2, 24, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString("M5StickC Plus2", SCREEN_W / 2, 50, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString("v1.8 Map Expansion", SCREEN_W / 2, 69, 2);

  frame.setTextColor(rgb(200, 200, 255), BLACK);
  frame.drawString("3 fases internas", SCREEN_W / 2, 87, 1);

  uint32_t pulse = (millis() / 280) % 2;
  frame.setTextColor(pulse ? YELLOW : rgb(180, 180, 180), BLACK);
  frame.drawString("Pressione A para iniciar", SCREEN_W / 2, 104, 2);

  frame.setTextColor(rgb(145, 90, 255), BLACK);
  frame.drawString("A=MOVER  B=TIRO  TILT=VIRAR", SCREEN_W / 2, 124, 1);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

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
<<<<<<< HEAD
=======
  frame.print("L");
  frame.print(currentLevel + 1);

  frame.setCursor(22, 5);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  frame.print("HP");

  if (player.hp <= 25) {
    frame.setTextColor(RED, hudBg);
  } else if (player.hp <= 55) {
    frame.setTextColor(YELLOW, hudBg);
  } else {
    frame.setTextColor(rgb(120, 255, 180), hudBg);
  }
<<<<<<< HEAD
  frame.setCursor(22, 5);
  frame.print(player.hp);

  // barra de vida compacta
  int hpBarW = map(player.hp, 0, PLAYER_MAX_HP, 0, 34);
  frame.drawRect(48, 5, 36, 7, rgb(75, 55, 95));
  frame.fillRect(49, 6, hpBarW, 5, player.hp <= 25 ? RED : rgb(80, 255, 130));

  frame.setTextColor(WHITE, hudBg);
  frame.setCursor(89, 5);
  frame.print("AM");
  frame.setTextColor(player.ammo <= 4 ? RED : WHITE, hudBg);
  frame.setCursor(108, 5);
  frame.print(player.ammo);

  frame.setTextColor(rgb(255, 100, 90), hudBg);
  frame.setCursor(138, 5);
=======
  frame.setCursor(40, 5);
  frame.print(player.hp);

  int hpBarW = map(player.hp, 0, PLAYER_MAX_HP, 0, 28);
  frame.drawRect(65, 5, 30, 7, rgb(75, 55, 95));
  frame.fillRect(66, 6, hpBarW, 5, player.hp <= 25 ? RED : rgb(80, 255, 130));

  frame.setTextColor(WHITE, hudBg);
  frame.setCursor(100, 5);
  frame.print("AM");
  frame.setTextColor(player.ammo <= 4 ? RED : WHITE, hudBg);
  frame.setCursor(119, 5);
  frame.print(player.ammo);

  frame.setTextColor(rgb(255, 100, 90), hudBg);
  frame.setCursor(148, 5);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  frame.print("EN ");
  frame.print(enemiesAliveCount());

  if (millis() < sprintUntilMs) {
    frame.setTextColor(YELLOW, hudBg);
<<<<<<< HEAD
    frame.setCursor(174, 5);
    frame.print("RUN");
  }

  frame.setTextColor(rgb(130, 255, 180), hudBg);
  frame.setCursor(205, 5);
=======
    frame.setCursor(181, 5);
    frame.print("R");
  }

  frame.setTextColor(rgb(130, 255, 180), hudBg);
  frame.setCursor(207, 5);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  frame.print(fpsValue);
  frame.print("F");
}

static void drawWeapon() {
  int cx = SCREEN_W / 2;
  int baseY = SCREEN_H - 1;

  uint16_t metal = rgb(46, 46, 58);
  uint16_t metal2 = rgb(82, 82, 98);
  uint16_t dark = rgb(18, 18, 26);
  uint16_t edge = rgb(135, 65, 245);

  frame.fillRoundRect(cx - 28, baseY - 35, 56, 37, 7, dark);
  frame.drawRoundRect(cx - 28, baseY - 35, 56, 37, 7, edge);
  frame.fillRoundRect(cx - 16, baseY - 29, 32, 29, 5, metal);
  frame.drawFastVLine(cx - 8, baseY - 26, 22, rgb(25, 25, 34));
  frame.drawFastVLine(cx + 8, baseY - 26, 22, rgb(25, 25, 34));
  frame.fillRect(cx - 7, baseY - 48, 14, 25, metal2);
  frame.fillRect(cx - 5, baseY - 53, 10, 8, rgb(105, 105, 125));

  if (millis() < shootFlashUntilMs) {
    frame.fillTriangle(cx, baseY - 80, cx - 23, baseY - 49, cx + 23, baseY - 49, YELLOW);
    frame.fillTriangle(cx, baseY - 70, cx - 12, baseY - 52, cx + 12, baseY - 52, WHITE);
  }

  uint16_t cross = rgb(170, 255, 210);
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
      int size = (int)(31.0f / dist);
      if (size < 4) size = 4;
      if (size > 19) size = 19;
      int sy = SCREEN_H / 2 + (int)(20.0f / dist);

      uint16_t col = WHITE;
      if (c == 'H') col = rgb(80, 255, 120);
      if (c == 'M') col = rgb(80, 150, 255);
      if (c == 'E') col = rgb(250, 230, 80);

      frame.drawCircle(sx, sy, size / 2 + 2, col);
      frame.fillRoundRect(sx - size / 2, sy - size / 2, size, size, 3, col);
      frame.setTextDatum(middle_center);
      frame.setTextSize(1);
      frame.setTextColor(BLACK, col);
      frame.drawString(String(c), sx, sy);
      frame.setTextDatum(top_left);
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
    int spriteH = (int)(70.0f / dist);
    if (spriteH < 6) spriteH = 6;
    if (spriteH > 60) spriteH = 60;
    int spriteW = spriteH / 2;

    int bob = (int)(sinf(enemies[i].anim) * 2.0f);
    int sy = SCREEN_H / 2 - spriteH / 2 + 8 + bob;

    uint16_t body = rgb(165, 55, 50);
    uint16_t body2 = rgb(120, 35, 35);
    uint16_t dark = rgb(60, 18, 20);
    uint16_t eye = rgb(255, 235, 90);

<<<<<<< HEAD
    // sombra
    frame.fillEllipse(sx, sy + spriteH + 3, max(3, spriteW), 3, rgb(18, 12, 18));

    // corpo
    frame.fillRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, body);
    frame.drawRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, dark);
    frame.fillRect(sx - spriteW / 2 + 2, sy + spriteH / 2, max(2, spriteW - 4), spriteH / 3, body2);

    // olhos/boca
    frame.fillCircle(sx - spriteW / 4, sy + spriteH / 4, 2, eye);
    frame.fillCircle(sx + spriteW / 4, sy + spriteH / 4, 2, eye);
    frame.drawFastHLine(sx - spriteW / 3, sy + spriteH / 2, max(2, spriteW * 2 / 3), dark);

    // chifres/braços fake
=======
    frame.fillEllipse(sx, sy + spriteH + 3, max(3, spriteW), 3, rgb(18, 12, 18));
    frame.fillRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, body);
    frame.drawRoundRect(sx - spriteW / 2, sy, spriteW, spriteH, 4, dark);
    frame.fillRect(sx - spriteW / 2 + 2, sy + spriteH / 2, max(2, spriteW - 4), spriteH / 3, body2);
    frame.fillCircle(sx - spriteW / 4, sy + spriteH / 4, 2, eye);
    frame.fillCircle(sx + spriteW / 4, sy + spriteH / 4, 2, eye);
    frame.drawFastHLine(sx - spriteW / 3, sy + spriteH / 2, max(2, spriteW * 2 / 3), dark);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
    frame.drawLine(sx - spriteW / 2, sy + 4, sx - spriteW, sy - 5, dark);
    frame.drawLine(sx + spriteW / 2, sy + 4, sx + spriteW, sy - 5, dark);
    frame.drawLine(sx - spriteW / 2, sy + spriteH / 2, sx - spriteW, sy + spriteH / 2 + 8, dark);
    frame.drawLine(sx + spriteW / 2, sy + spriteH / 2, sx + spriteW, sy + spriteH / 2 + 8, dark);
  }
}

static void renderWorld() {
  frame.fillScreen(BLACK);

<<<<<<< HEAD
  // céu/chão
  frame.fillRect(0, 19, SCREEN_W, (SCREEN_H - 19) / 2, rgb(20, 20, 40));
  frame.fillRect(0, 19 + (SCREEN_H - 19) / 2, SCREEN_W, (SCREEN_H - 19) / 2, rgb(28, 24, 23));

  // chão fake
=======
  frame.fillRect(0, 19, SCREEN_W, (SCREEN_H - 19) / 2, rgb(20, 20, 40));
  frame.fillRect(0, 19 + (SCREEN_H - 19) / 2, SCREEN_W, (SCREEN_H - 19) / 2, rgb(28, 24, 23));

>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  for (int y = SCREEN_H / 2 + 10; y < SCREEN_H; y += 10) {
    frame.drawFastHLine(0, y, SCREEN_W, rgb(45, 35, 55));
  }

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

    float shadeF = 218.0f - corrected * 25.0f;
    if (shadeF < 18.0f) shadeF = 18.0f;
    if (shadeF > 240.0f) shadeF = 240.0f;
    uint8_t shade = (uint8_t)shadeF;

    uint16_t wallCol;
    if (hitCell == 'D') {
      wallCol = rgb(shade, (uint8_t)(shade * 0.58f), 30);
    } else {
<<<<<<< HEAD
      wallCol = rgb((uint8_t)(shade * 0.40f), (uint8_t)(shade * 0.34f), shade);
=======
      uint8_t phase = (uint8_t)(currentLevel * 16);
      wallCol = rgb((uint8_t)(shade * 0.40f + phase), (uint8_t)(shade * 0.34f), shade);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
    }

    frame.fillRect(x, ceiling, RAY_COLUMN_STEP, floorY - ceiling, wallCol);

<<<<<<< HEAD
    // textura fake: linhas verticais + blocos
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
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

  // barra/status inferior
  frame.setTextDatum(bottom_left);
  frame.setTextSize(1);
  frame.setTextColor(rgb(150, 255, 190), BLACK);
<<<<<<< HEAD
  frame.fillRect(0, SCREEN_H - 11, 160, 11, BLACK);
=======
  frame.fillRect(0, SCREEN_H - 11, 170, 11, BLACK);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

  if (millis() > statusUntilMs) {
    statusLine = imuReady ? "Pronto" : "IMU sem leitura";
  }

  frame.drawString(statusLine, 4, SCREEN_H - 1);

<<<<<<< HEAD
  // efeito de pickup
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  if (millis() < pickupFlashUntilMs) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, rgb(120, 255, 180));
  }

<<<<<<< HEAD
  // efeito de munição vazia
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  if (millis() < emptyAmmoFlashUntilMs) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, YELLOW);
    frame.drawRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, YELLOW);
  }

<<<<<<< HEAD
  // pulso visual de HP baixo
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  if (player.hp > 0 && player.hp <= 25 && ((millis() / 260) % 2 == 0)) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, rgb(130, 20, 20));
  }

<<<<<<< HEAD
  // efeito de dano
=======
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  if (millis() < damageFlashUntilMs) {
    frame.drawRect(0, 0, SCREEN_W, SCREEN_H, RED);
    frame.drawRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, RED);
    frame.drawRect(2, 2, SCREEN_W - 4, SCREEN_H - 4, RED);
  }
}

static void drawLevelClearScreen() {
  frame.fillScreen(BLACK);
  drawCyberGrid();
<<<<<<< HEAD
  drawFrameBorder(titleColor);
=======
  drawFrameBorder(rgb(120, 255, 180));
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(155, 90, 255), BLACK);
<<<<<<< HEAD
  frame.drawString("DOOMSTICKC v1.7", SCREEN_W / 2, 20, 2);
=======
  frame.drawString("DOOMSTICKC v1.8", SCREEN_W / 2, 18, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString("FASE OK", SCREEN_W / 2, 50, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString("Preparando fase " + String(currentLevel + 2), SCREEN_W / 2, 84, 2);

  frame.setTextColor(YELLOW, BLACK);
  frame.drawString("+HP  +AMMO", SCREEN_W / 2, 110, 2);

  frame.pushSprite(0, 0);
}

static void drawEndScreen(const char* title, const char* subtitle, uint16_t titleColor) {
  frame.fillScreen(BLACK);
  drawCyberGrid();
  drawFrameBorder(titleColor);

  frame.setTextDatum(middle_center);
  frame.setTextSize(1);

  frame.setTextColor(rgb(155, 90, 255), BLACK);
  frame.drawString("DOOMSTICKC v1.8", SCREEN_W / 2, 20, 2);
>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)

  frame.setTextColor(titleColor, BLACK);
  frame.drawString(title, SCREEN_W / 2, 56, 4);

  frame.setTextColor(WHITE, BLACK);
  frame.drawString(subtitle, SCREEN_W / 2, 88, 2);

  frame.setTextColor(rgb(120, 255, 180), BLACK);
  frame.drawString("A + B para reiniciar", SCREEN_W / 2, 113, 2);

  frame.pushSprite(0, 0);
}

static void renderFrame() {
  if (gameState == GAME_INTRO) {
    drawIntroScreen();
    return;
  }

<<<<<<< HEAD
=======
  if (gameState == GAME_LEVEL_CLEAR) {
    drawLevelClearScreen();
    return;
  }

>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  if (gameState == GAME_DEAD) {
    drawEndScreen("VOCE MORREU", "Tente novamente", RED);
    return;
  }

  if (gameState == GAME_WIN) {
    drawEndScreen("CAMPANHA OK", "3 fases concluidas", rgb(120, 255, 180));
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

  if (gameState == GAME_INTRO) {
    if (M5.BtnA.wasPressed() && millis() - bootMs > INTRO_MIN_MS) {
      playToneSafe(TONE_START, 100);
      resetGame(true);
    }
    return;
  }

<<<<<<< HEAD
=======
  if (gameState == GAME_LEVEL_CLEAR) {
    if (millis() - levelClearStartedMs >= LEVEL_CLEAR_PAUSE_MS) {
      loadLevel(currentLevel + 1, true);
    }
    return;
  }

>>>>>>> 3fd9055 (Initial DoomStickC v1.7 audio feedback)
  if (gameState != GAME_PLAYING) {
    if (M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
      resetGame(true);
    }
    return;
  }

  if (M5.BtnB.wasPressed()) {
    shoot();
  }

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

  if (AUDIO_ENABLED) {
    M5.Speaker.setVolume(AUDIO_VOLUME);
  }

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
