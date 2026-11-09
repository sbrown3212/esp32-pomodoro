#include <Arduino.h>
#include <Bounce2.h>
#include <U8g2lib.h>
#include <esp_timer.h>

#include <chrono>

#include "pins.h"
#include "timer_logic.h"

namespace {

constexpr uint16_t DEBOUNCE_MS = 10;

// Below U8g2's SSD1306 default of 207, because OLED wear rises steeply with brightness.
// Tuned by eye on the device: 64, 48 and 16 looked about the same, so the lowest was kept.
constexpr uint8_t DISPLAY_CONTRAST = 16;

constexpr int MARGIN_PX = 2;
constexpr int HEADER_BASELINE_Y = 11;
constexpr int TIME_BASELINE_Y = 48;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, PIN_DISPLAY_SCL, PIN_DISPLAY_SDA);
Bounce2::Button primaryButton;
Bounce2::Button nextButton;

TimerLogic timer;
Snapshot drawnSnapshot;

void setUpButton(Bounce2::Button &button, uint8_t pin) {
  button.attach(pin, INPUT_PULLUP);
  button.interval(DEBOUNCE_MS);
  button.setPressedState(LOW);
}

// The one place a clock is read. esp_timer_get_time() is the 64-bit microsecond clock since boot.
TimePoint deviceNow() {
  const std::chrono::duration<int64_t, std::micro> sinceBoot(esp_timer_get_time());
  return TimePoint(std::chrono::duration_cast<Duration>(sinceBoot));
}

const char *intervalTypeLabel(IntervalType type) {
  return type == IntervalType::Focus ? "FOCUS" : "BREAK";
}

const char *runStateLabel(RunState state) {
  switch (state) {
    case RunState::Ready:
      return "READY";
    case RunState::Running:
      return "RUNNING";
    case RunState::Paused:
      return "PAUSED";
  }
  return "";
}

void draw(const Snapshot &snapshot) {
  display.clearBuffer();

  display.setFont(u8g2_font_7x13B_tr);
  display.drawStr(MARGIN_PX, HEADER_BASELINE_Y, intervalTypeLabel(snapshot.intervalType));
  const char *runState = runStateLabel(snapshot.runState);
  display.drawStr(display.getDisplayWidth() - MARGIN_PX - display.getStrWidth(runState), HEADER_BASELINE_Y, runState);

  char timeText[12];
  snprintf(timeText, sizeof(timeText), "%02lu:%02lu", static_cast<unsigned long>(snapshot.secondsToShow / 60),
           static_cast<unsigned long>(snapshot.secondsToShow % 60));
  // Centre on a fixed "00:00", not the actual text. Every digit in this font advances the same
  // distance, but getStrWidth() measures the first and last glyphs by their visible pixels, so
  // a leading or trailing '1' would change the width and shift the time as it counts.
  display.setFont(u8g2_font_logisoso28_tn);
  display.drawStr((display.getDisplayWidth() - display.getStrWidth("00:00")) / 2, TIME_BASELINE_Y, timeText);

  display.sendBuffer();
}

}  // namespace

void setup() {
  setUpButton(primaryButton, PIN_BUTTON_PRIMARY);
  setUpButton(nextButton, PIN_BUTTON_NEXT);

  display.begin();
  display.setContrast(DISPLAY_CONTRAST);
  drawnSnapshot = timer.snapshot();
  draw(drawnSnapshot);
}

void loop() {
  const TimePoint now = deviceNow();

  primaryButton.update();
  nextButton.update();

  if (primaryButton.pressed()) {
    timer.primaryPressed(now);
  }
  if (nextButton.pressed()) {
    timer.nextPressed(now);
  }
  timer.tick(now);

  const Snapshot snapshot = timer.snapshot();
  if (snapshot != drawnSnapshot) {
    draw(snapshot);
    drawnSnapshot = snapshot;
  }
}
