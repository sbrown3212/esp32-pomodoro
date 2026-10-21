# 01: Hello world: toolchain, display and buttons verified on the device

**What to build:** A minimal firmware that proves the whole toolchain and all the wiring work before any timer logic exists. On power-up the OLED shows a hello message plus a press count for each button. Pressing Primary or Next increments its count by exactly one per press. Separately, the host test runner runs and passes a placeholder test. Spec: `.scratch/mvp/spec.md`. Toolchain: ADR 0001.

**Blocked by:** None (can start immediately)

**Status:** ready-for-agent

- [ ] PlatformIO project with the Arduino framework and two environments: the ESP32 DevKit V1 board, and native (host)
- [ ] The native environment runs a placeholder Unity test that passes, with no board attached
- [ ] Firmware builds and uploads to the DevKit V1
- [ ] The SSD1306 128×64 (I2C, SDA GPIO21 / SCL GPIO22) shows a hello message, rendered with U8g2
- [ ] Primary (GPIO25) and Next (GPIO26) are wired to GND with internal pull-ups and debounced via a button library
- [ ] The screen shows a separate press count for Primary and Next; one physical press increments its count by exactly one (checked by hand, including rapid presses)
- [ ] All pin assignments are defined in a single place
- [ ] Short note in the repo on how to build, upload and run the tests
