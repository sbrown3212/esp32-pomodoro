# 01: Hello world: toolchain, display and buttons verified on the device

**What to build:** A minimal firmware that proves the whole toolchain and all the wiring work before any timer logic exists. On power-up the OLED shows a hello message plus a press count for each button. Pressing Primary or Next increments its count by exactly one per press. Separately, the host test runner runs and passes a placeholder test. Spec: `.scratch/mvp/spec.md`. Toolchain: ADR 0001.

**Blocked by:** None (can start immediately)

**Status:** resolved

- [x] PlatformIO project with the Arduino framework and two environments: the ESP32 DevKit V1 board, and native (host)
- [x] The native environment runs a placeholder Unity test that passes, with no board attached
- [x] Firmware builds and uploads to the DevKit V1
- [x] The SSD1306 128×64 (I2C, SDA GPIO21 / SCL GPIO22) shows a hello message, rendered with U8g2
- [x] Primary (GPIO25) and Next (GPIO26) are wired to GND with internal pull-ups and debounced via a button library
- [x] The screen shows a separate press count for Primary and Next; one physical press increments its count by exactly one (checked by hand, including rapid presses)
- [x] All pin assignments are defined in a single place
- [x] Short note in the repo on how to build, upload and run the tests

## Comments

Firmware is written and builds, but has not run on hardware: no board was attached. The unticked items need checking on the device: upload, the hello message, and the press counts, including rapid presses. Include very short taps alternating between the two buttons: buttons aren't read during a redraw (estimated at about 30 ms, not measured), so a tap that fits entirely inside one could be missed. Also watch for double counts, which would mean a switch bounces for longer than the 10 ms debounce.

All checks passed on the device: upload, the hello message, and the press counts. Quick runs of 20 taps on Primary, on Next, and alternating between the two each matched the expected counts, with no missed or double counts.
