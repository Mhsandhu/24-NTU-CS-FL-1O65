<div align="center">

# Embedded IoT Systems — Homework 2
### Dual-LED PWM Control with Real-Time OLED Monitoring

<br />

<!-- Student & Course Metadata SVG Badges -->
![Student](https://img.shields.io/badge/Student-Muhammad%20Hasnain%20Anjum-0f172a?style=for-the-badge&logo=github&logoColor=38bdf8&labelColor=1e293b)
![Reg No](https://img.shields.io/badge/Reg%20No.-24--NTU--CS--FL--1065-0f172a?style=for-the-badge&logo=target&logoColor=38bdf8&labelColor=1e293b)
![Course](https://img.shields.io/badge/Course-Embedded%20IoT%20Systems-0f172a?style=for-the-badge&logo=espressif&logoColor=38bdf8&labelColor=1e293b)

<!-- Hardware & Tech Stack SVG Badges -->
![ESP32](https://img.shields.io/badge/ESP32_DevKit-E7352C?style=flat-square&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=flat-square&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Arduino](https://img.shields.io/badge/Framework-Arduino-00878F?style=flat-square&logo=arduino&logoColor=white)
![OLED](https://img.shields.io/badge/Display-SSD1306_I2C-38bdf8?style=flat-square)

<br />

[Overview](#1-project-overview) •
[Components](#2-components-used) •
[Circuit Pinout](#3-circuit-connections) •
[Task 1: PWM](#4-task-1--dual-led-pwm-fading) •
[Task 2: OLED](#5-task-2--oled-monitoring) •
[Verification](#6-testing-and-results)

</div>

---

## 1. Project Overview

![Section](https://img.shields.io/badge/Section-Overview-1e293b?style=flat-square&logo=readme&logoColor=38bdf8)

In this homework, I used an **ESP32 DevKit** to control two LEDs using **Pulse Width Modulation (PWM)**. Both LEDs gradually fade in opposite directions while operating at different PWM frequencies (**1000 Hz** and **5000 Hz**).

I also integrated an **SSD1306 OLED Display (128×64)** over I²C to monitor the live duty-cycle percentages and configured frequencies of both LEDs in real time.

The complete project was tested and verified in **Wokwi Simulation** and on **physical ESP32 hardware**.

---

## 2. Components Used

![Section](https://img.shields.io/badge/Section-Hardware_Bill_of_Materials-1e293b?style=flat-square&logo=espressif&logoColor=38bdf8)

| Component | Quantity | Specifications / Description |
| :--- | :---: | :--- |
| **ESP32 DevKit** | `1` | Microcontroller board with hardware LEDC PWM and I²C |
| **LEDs (Red & Blue)** | `2` | 1× Red LED (Channel 1), 1× Blue LED (Channel 2) |
| **Resistors** | `2` | 220 Ω current-limiting resistors (connected in series) |
| **SSD1306 OLED Display** | `1` | 128×64 pixel resolution, I²C interface |
| **Breadboard & Jumper Wires** | `1 Set` | For circuit prototyping and connections |
| **USB Cable** | `1` | For power supply and firmware uploading |

---

## 3. Circuit Connections

![Section](https://img.shields.io/badge/Section-Hardware_Wiring_&_Pinout-1e293b?style=flat-square&logo=circuitverse&logoColor=38bdf8)

Each LED has its own **220 Ω current-limiting resistor** connected in series, and both LED cathodes are connected to common ground (`GND`).

| Component / Pin | ESP32 GPIO / Pin | Connection Details |
| :--- | :---: | :--- |
| **Red LED (Anode)** | `GPIO 18` | Connected via 220 Ω series resistor; Cathode to `GND` |
| **Blue LED (Anode)** | `GPIO 19` | Connected via 220 Ω series resistor; Cathode to `GND` |
| **OLED SDA** | `GPIO 21` | I²C Serial Data line |
| **OLED SCL** | `GPIO 22` | I²C Serial Clock line |
| **OLED VCC** | `3V3` | 3.3V Power supply |
| **OLED GND** | `GND` | Common Ground |

<details>
<summary><b>Click to view Block Wiring Diagram</b></summary>
<br />

```text
                       +-----------------------+
                       |     ESP32 DEVKIT      |
                       |                       |
                       |               GPIO 18 |----[ 220Ω ]---->|----( GND )  [Red LED]
                       |               GPIO 19 |----[ 220Ω ]---->|----( GND )  [Blue LED]
                       |                       |
                       |               GPIO 21 |---------------------> SDA     [SSD1306 OLED]
                       |               GPIO 22 |---------------------> SCL     [SSD1306 OLED]
                       |                   3V3 |---------------------> VCC     [SSD1306 OLED]
                       |                   GND |---------------------> GND     [SSD1306 OLED]
                       +-----------------------+