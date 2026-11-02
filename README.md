# ESP32 Pomodoro

A desktop Pomodoro timer on an ESP32 DevKit V1 with a 128×64 OLED and two buttons. See `.scratch/mvp/spec.md`.

## Setup

Install [PlatformIO Core](https://docs.platformio.org/en/latest/core/), for example with `uv tool install platformio`.

## Commands

| Task | Command |
|---|---|
| Run the tests on the host (no board needed) | `pio test -e native` |
| Build the firmware | `pio run` |
| Upload to a connected board | `pio run -t upload` |

Pin assignments are in `include/pins.h`.
