#pragma once

/*
  DoomStickCUI
  v4.0 Full Evolution

  Adiciona textos para: dificuldade, score, sala secreta, Shotgun.
*/

namespace DoomStickCUI {
  // Loading screen
  static constexpr const char* LOADING_TOP_LABEL  = "M5STICKC PLUS2";
  static constexpr const char* LOADING_BRAND_LEFT  = "Peek";
  static constexpr const char* LOADING_BRAND_RIGHT = "Security";
  static constexpr const char* LOADING_TEXT        = "LOADING...";
  static constexpr const char* LOADING_BOOT_0      = "> BOOTING SYSTEMS...";
  static constexpr const char* LOADING_BOOT_1      = "> INIT ENGINE...";
  static constexpr const char* LOADING_BOOT_2      = "> LOAD MAPS...";
  static constexpr const char* LOADING_BOOT_3      = "> PEEK READY...";
  static constexpr const char* LOADING_OK          = "[OK]";

  // Intro / controls
  static constexpr const char* INTRO_START         = "Pressione A para iniciar";
  static constexpr const char* INTRO_CONTROLS      = "A=MOVER  B=TIRO  TILT=VIRAR";
  static constexpr const char* INTRO_DIFF_HINT     = "Power=DIFICULDADE";

  // HUD
  static constexpr const char* HUD_LEVEL   = "L";
  static constexpr const char* HUD_HP      = "HP";
  static constexpr const char* HUD_AMMO    = "AM";
  static constexpr const char* HUD_ENEMIES = "EN ";
  static constexpr const char* HUD_RUN     = "R";
  static constexpr const char* HUD_FPS     = "F";
  static constexpr const char* HUD_WEAPON  = "W";
  static constexpr const char* HUD_KILLS   = "K";

  // Status messages
  static constexpr const char* STATUS_READY          = "Pronto";
  static constexpr const char* STATUS_IMU_MISSING    = "IMU sem leitura";
  static constexpr const char* STATUS_LEVEL_PREFIX   = "Fase ";
  static constexpr const char* STATUS_CAMPAIGN_DONE  = "Campanha concluida";
  static constexpr const char* STATUS_LEVEL_DONE     = "Fase concluida";
  static constexpr const char* STATUS_HEALTH_PICKUP  = "+30 Vida";
  static constexpr const char* STATUS_AMMO_PICKUP    = "+12 Municao";
  static constexpr const char* STATUS_DOOR_OPEN      = "Porta aberta";
  static constexpr const char* STATUS_NOTHING_TO_USE = "Nada para usar";
  static constexpr const char* STATUS_EMPTY_AMMO     = "Sem municao";
  static constexpr const char* STATUS_ENEMY_DOWN     = "Inimigo abatido";
  static constexpr const char* STATUS_SHOT           = "Tiro";
  static constexpr const char* STATUS_WEAPON_PREFIX  = "Arma: ";
  static constexpr const char* STATUS_WEAPON_PICKUP  = "Blaster coletada";
  static constexpr const char* STATUS_SHOTGUN_PICKUP = "Shotgun coletada";
  static constexpr const char* STATUS_RUN            = "Corrida ativada";
  static constexpr const char* STATUS_DAMAGE         = "-7 Vida";
  static constexpr const char* STATUS_DEAD           = "Voce morreu";
  static constexpr const char* STATUS_GOOD_LUCK      = "Boa sorte";
  static constexpr const char* STATUS_SECRET         = "Sala secreta!";
  static constexpr const char* STATUS_TANK_HIT       = "Tank atingido";
  static constexpr const char* STATUS_RANGED_SHOT    = "Projétil inimigo";

  // Level/final screens
  static constexpr const char* LEVEL_CLEAR_TITLE      = "FASE OK";
  static constexpr const char* LEVEL_CLEAR_PREFIX     = "Preparando fase ";
  static constexpr const char* LEVEL_CLEAR_BONUS      = "+HP  +AMMO";
  static constexpr const char* LEVEL_START_TITLE_PREFIX = "LEVEL ";
  static constexpr const char* LEVEL_START_OBJECTIVE  = "Objetivo: encontre o E";
  static constexpr const char* LEVEL_START_HINT       = "Prepare-se";
  static constexpr const char* DEAD_TITLE             = "VOCE MORREU";
  static constexpr const char* DEAD_SUBTITLE          = "Tente novamente";
  static constexpr const char* WIN_TITLE              = "CAMPANHA OK";
  static constexpr const char* WIN_SUBTITLE           = "5 fases concluidas";
  static constexpr const char* RESTART_HINT           = "A + B para reiniciar";
  static constexpr const char* SCORE_PREFIX           = "SCORE: ";
  static constexpr const char* KILLS_PREFIX           = "KILLS: ";
  static constexpr const char* TIME_PREFIX            = "TEMPO: ";

  // Dificuldade
  static constexpr const char* DIFF_EASY   = "EASY";
  static constexpr const char* DIFF_NORMAL = "NORMAL";
  static constexpr const char* DIFF_HARD   = "HARD";
  static constexpr const char* DIFF_PREFIX = "DIFICULDADE: ";

  static constexpr unsigned long LOADING_SCREEN_MS   = 2600;
  static constexpr unsigned long LOADING_MIN_SKIP_MS = 1200;
}
