#include "doomstickc/DoomStickCAudio.h"

namespace DoomStickCAudio {
  void begin() {
    if (AUDIO_ENABLED) {
      M5.Speaker.setVolume(AUDIO_VOLUME);
    }
  }

  void playToneSafe(int freq, int durationMs) {
    if (!AUDIO_ENABLED) return;
    if (freq <= 0 || durationMs <= 0) return;
    M5.Speaker.tone(freq, durationMs);
  }
}
