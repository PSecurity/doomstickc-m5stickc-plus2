#pragma once

/*
  DoomStickC Version Metadata
  v1.9 Code Organization Foundation

  This small module centralizes version/project labels.
  It is the first safe step toward splitting the engine into modules later:
    - config
    - map
    - player
    - enemies
    - render
    - audio
    - ui

  The gameplay remains preserved from v1.8.
*/

namespace DoomStickCVersion {
  static constexpr const char* APP_NAME = "DoomStickC";
  static constexpr const char* VERSION = "v1.9";
  static constexpr const char* CODENAME = "Code Organization";
  static constexpr const char* TITLE = "DOOMSTICKC";
  static constexpr const char* HARDWARE = "M5StickC Plus2";
  static constexpr const char* INTRO_LABEL = "v1.9 Code Org";
  static constexpr const char* CALIBRATION_LABEL = "v1.9 Code Org";
  static constexpr const char* STATUS_LABEL = "DoomStickC v1.9";
}
