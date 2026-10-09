#include <Arduino.h>

#define LED 13
#define BUTTON_PIN 32

hw_timer_t *debounceTimer = NULL;

volatile bool debounceActive = false;
volatile bool ledState = false;

// Button dabate hi chalta hai (FALLING edge)
void IRAM_ATTR onButtonISR()
{
  if (!debounceActive)
  {
    debounceActive = true;              // lock lagao
    timerWrite(debounceTimer, 0);       // timer 0 se shuru
    timerAlarmEnable(debounceTimer);    // 50 ms baad alarm
  }
}

// 50 ms baad chalta hai
void IRAM_ATTR onDebounceTimer()
{
  if (digitalRead(BUTTON_PIN) == LOW)   // abhi bhi dabaa hai = asli press
  {
    ledState = !ledState;
    digitalWrite(LED, ledState);
  }
  debounceActive = false;               // lock hatao
}

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  debounceTimer = timerBegin(1, 80, true);              // 1 MHz tick
  timerAttachInterrupt(debounceTimer, &onDebounceTimer, true);
  timerAlarmWrite(debounceTimer, 50000, false);         // 50 ms, one-shot

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonISR, FALLING);
}

void loop()
{
}