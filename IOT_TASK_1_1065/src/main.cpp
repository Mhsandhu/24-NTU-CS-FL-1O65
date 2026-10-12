#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;
constexpr unsigned long BLINK_INTERVAL_MS = 1000;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    Serial.begin(115200);
    Serial.println("IOT TASK 1 - ESP32 LED BLINK");
    Serial.println("Registration: 24-NTU-CS-FL-1065");
}

void loop()
{
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED: ON");
    delay(BLINK_INTERVAL_MS);

    digitalWrite(LED_PIN, LOW);
    Serial.println("LED: OFF");
    delay(BLINK_INTERVAL_MS);
}