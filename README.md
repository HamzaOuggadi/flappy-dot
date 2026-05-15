# 🐦 Flappy Dot: Bare-Metal ATmega32 Game

A lightweight, one-button arcade game built entirely from scratch on a bare-metal Atmel ATmega32 microcontroller. 

Instead of relying on a development board like an Arduino Uno, this project uses a standalone ATmega32 DIP-40 chip on a breadboard, complete with its own 16MHz clock and power circuitry. It's a great demonstration of bare-metal AVR programming, hardware debouncing, physics loops, and I2C graphics.

![Project Cover Photo](gameImage.jpg)

## ✨ Features
* **Custom Physics Engine:** Gravity and velocity math optimized for an 8-bit microcontroller.
* **Collision Detection:** Real-time hitbox calculation for the player and moving gaps.
* **Audio Feedback:** Piezo buzzer integration for jump, score, and game-over sound effects.
* **OLED Graphics:** 128x64 display rendering using the I2C protocol.
* **Bare-Metal Setup:** Custom 16MHz crystal oscillator circuitry.

## 🛠️ Hardware Requirements
* 1x ATmega32 (DIP-40 Package)
* 1x 16MHz Crystal Oscillator
* 2x 22pF Ceramic Capacitors
* 1x 10kΩ Resistor
* 1x 0.96" OLED Display I2C (SSD1306)
* 1x Push Button
* 1x Piezo Buzzer
* 1x MB102 Breadboard Power Supply Module (Set to 5V)
* Breadboard & Jumper Wires
* *For Programming:* Arduino Uno (used as ISP) or USBasp.

## 🔌 Wiring & Schematic

*(flappy_dot.svg)*
`![Wiring Diagram](placeholder_for_diagram.png)`

### Pinout Mapping
| Component | ATmega32 Pin | AVR Port | Notes |
| :--- | :--- | :--- | :--- |
| **16MHz Crystal** | Pins 12 & 13 | XTAL1 / XTAL2 | With 22pF caps to Ground |
| **OLED SCL** | Pin 22 | PC0 | I2C Clock |
| **OLED SDA** | Pin 23 | PC1 | I2C Data |
| **Jump Button** | Pin 16 | PD2 | Active LOW (Internal Pull-up) |
| **Piezo Buzzer**| Pin 17 | PD3 | To Ground |

## 💻 Software & Toolchain Setup

This project is written in C/C++ using the Arduino framework, but compiled specifically for the bare ATmega32.

### Dependencies
1. **MightyCore:** Hardware package for the Arduino IDE to support the ATmega32.
   * Add `https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json` to your Arduino Boards Manager URLs.
2. **Libraries:** `Adafruit GFX` and `Adafruit SSD1306`.

### Installation & Flashing
1. Wire the ATmega32 to an ISP programmer (e.g., Arduino as ISP).
2. In the Arduino IDE, set:
   * **Board:** ATmega32 (MightyCore)
   * **Clock:** 16 MHz external
   * **Pinout:** Standard
3. Click **Tools > Burn Bootloader** (Only required once for a new chip to set the 16MHz fuses).
4. Compile the `flappy_dot.ino` sketch and upload it via **Sketch > Upload Using Programmer**.

## 🚀 Future Roadmap
- [ ] **Battery Power:** Integrate an 18650 Lithium battery with a TP4056 charging module to make the console fully portable.
- [ ] **High Scores:** Save high scores to the ATmega32's internal EEPROM.
- [ ] **Sprite Upgrades:** Replace primitive shapes with bitmap sprites.
