#include <Arduino.h>

#define LED_PIN 25
#define PWM_CH 0

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 PWM LED Test");

    ledcSetup(PWM_CH, 5000, 8);   // channel, freq, 8-bit resolution
    ledcAttachPin(LED_PIN, PWM_CH);
}

void loop()
{
    for (int v = 0; v <= 255; v++) { ledcWrite(PWM_CH, v); delay(10); }
    for (int v = 255; v >= 0; v--) { ledcWrite(PWM_CH, v); delay(10); }
}