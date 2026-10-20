# PlatformIO with the Arduino framework

We build with PlatformIO using the Arduino framework, rather than ESP-IDF or the Arduino IDE. PlatformIO's `native` environment lets the timer logic be unit-tested on the host machine without hardware, which supports test-first and AI-assisted development. The Arduino framework gives mature SSD1306 display libraries and also targets the ESP32-C3, a candidate for a later, more polished version. ESP-IDF was rejected as too much boilerplate for a project this size, and the Arduino IDE because it is weak for automated testing.
