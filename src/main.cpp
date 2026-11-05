#include <Arduino.h>
#include <Bounce2.h>
#include <U8g2lib.h>
#include <esp_timer.h>

#include "pins.h"
#include "timer_logic.h"

namespace {

constexpr uint16_t DEBOUNCE_MS = 10;

// Below U8g2's SSD1306 default of 207, because OLED wear rises steeply with brightness.
constexpr uint8_t DISPLAY_CONTRAST = 64;

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

// esp_timer_get_time() is the 64-bit microsecond clock since boot.
uint64_t nowMs() {
  return static_cast<uint64_t>(esp_timer_get_time()) / 1000;
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

  display.setFont(u8g2_font_6x13_tr);
  display.drawStr(MARGIN_PX, HEADER_BASELINE_Y, intervalTypeLabel(snapshot.intervalType));
  const char *runState = runStateLabel(snapshot.runState);
  display.drawStr(display.getDisplayWidth() - MARGIN_PX - display.getStrWidth(runState), HEADER_BASELINE_Y, runState);

  char time[12];
  snprintf(time, sizeof(time), "%02lu:%02lu", static_cast<unsigned long>(snapshot.secondsToShow / 60),
           static_cast<unsigned long>(snapshot.secondsToShow % 60));
  // Every digit in this font has the same width, so centring doesn't shift the time as it counts.
  display.setFont(u8g2_font_logisoso28_tn);
  display.drawStr((display.getDisplayWidth() - display.getStrWidth(time)) / 2, TIME_BASELINE_Y, time);

  display.sendBuffer();
  drawnSnapshot = snapshot;
}

}  // namespace

void setup() {
  setUpButton(primaryButton, PIN_BUTTON_PRIMARY);
  setUpButton(nextButton, PIN_BUTTON_NEXT);

  display.begin();
  display.setContrast(DISPLAY_CONTRAST);
  draw(timer.snapshot());
}

void loop() {
  const uint64_t now = nowMs();

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
  }
}
