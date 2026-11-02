#include <Arduino.h>
#include <Bounce2.h>
#include <U8g2lib.h>

#include "pins.h"

namespace {

constexpr uint16_t DEBOUNCE_MS = 10;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, PIN_DISPLAY_SCL, PIN_DISPLAY_SDA);
Bounce2::Button primaryButton;
Bounce2::Button nextButton;

uint32_t primaryCount = 0;
uint32_t nextCount = 0;

void setUpButton(Bounce2::Button &button, uint8_t pin) {
  button.attach(pin, INPUT_PULLUP);
  button.interval(DEBOUNCE_MS);
  button.setPressedState(LOW);
}

void drawCount(int y, const char *label, uint32_t count) {
  char line[24];
  snprintf(line, sizeof(line), "%s: %lu", label, static_cast<unsigned long>(count));
  display.drawStr(2, y, line);
}

void draw() {
  display.clearBuffer();
  display.drawStr(2, 14, "Hello, Pomodoro!");
  drawCount(36, "Primary", primaryCount);
  drawCount(52, "Next", nextCount);
  display.sendBuffer();
}

}  // namespace

void setup() {
  setUpButton(primaryButton, PIN_BUTTON_PRIMARY);
  setUpButton(nextButton, PIN_BUTTON_NEXT);

  display.begin();
  display.setFont(u8g2_font_6x13_tr);
  draw();
}

void loop() {
  primaryButton.update();
  nextButton.update();

  bool changed = false;
  if (primaryButton.pressed()) {
    primaryCount++;
    changed = true;
  }
  if (nextButton.pressed()) {
    nextCount++;
    changed = true;
  }

  if (changed) {
    draw();
  }
}
