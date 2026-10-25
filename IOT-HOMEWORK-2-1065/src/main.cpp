/*
  Embedded IoT Systems - Homework 02, Task 1
  Dual LED PWM fading on ESP32

  LED 1 (red)  : GPIO 18, 1000 Hz
  LED 2 (blue) : GPIO 19, 5000 Hz
  Both use 8-bit PWM (values 0 to 255) and fade in opposite directions.
*/
#include <Arduino.h>

const int LED1_PIN  = 18;     // red LED
const int LED2_PIN  = 19;     // blue LED
const int LED1_FREQ = 1000;   // Hz
const int LED2_FREQ = 5000;   // Hz
const int PWM_RES   = 8;      // 8-bit: 0 to 255

// The LEDC functions changed in Arduino-ESP32 version 3.
// These lines make the same code work with both versions.
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  #define LED1_CH LED1_PIN    // new core: write using the pin
  #define LED2_CH LED2_PIN
#else
  #define LED1_CH 0           // old core: write using the channel
  #define LED2_CH 1
#endif

int level = 0;    // brightness of LED 1 (0 to 255)
int dir   = 1;    // +1 = getting brighter, -1 = getting dimmer

void setup() {
  Serial.begin(115200);

  // set frequency and resolution, and connect each PWM to its pin
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(LED1_PIN, LED1_FREQ, PWM_RES);
  ledcAttach(LED2_PIN, LED2_FREQ, PWM_RES);
#else
  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);
#endif

  ledcWrite(LED1_CH, 0);      // initial duty value of LED 1
  ledcWrite(LED2_CH, 255);    // initial duty value of LED 2
}

void loop() {
  // change the brightness a little, and turn around at 0 and 255
  level = level + dir * 5;
  if (level >= 255) { level = 255; dir = -1; }
  if (level <= 0)   { level = 0;   dir = 1;  }

  int duty1 = level;          // LED 1 gets brighter ...
  int duty2 = 255 - level;    // ... while LED 2 gets dimmer

  ledcWrite(LED1_CH, duty1);
  ledcWrite(LED2_CH, duty2);

  // duty cycle (%) = duty / 255 x 100
  int percent1 = duty1 * 100 / 255;
  int percent2 = duty2 * 100 / 255;

  Serial.print("LED1: ");
  Serial.print(percent1);
  Serial.print("%   LED2: ");
  Serial.print(percent2);
  Serial.println("%");

  delay(30);
}