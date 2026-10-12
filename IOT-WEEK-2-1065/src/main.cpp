// Week 2 - Button Controlled Dual LED Task
// Embedded IoT System Fall-2026
// Muhammad Hasnain Anjum  // 24-NTU-CS-FL-1065

#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 36;
constexpr uint8_t LED_PIN = 2;
constexpr uint8_t LED2_PIN = 5;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
    const int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH)
    {
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(LED2_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LED2_PIN, HIGH);
    }
}
