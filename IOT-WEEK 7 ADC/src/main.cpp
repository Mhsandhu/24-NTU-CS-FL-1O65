#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LDR_PIN 4           // ADC2 pin (AO wire yahin lagi hai, WiFi use nahi karna)
#define SDA_PIN 21
#define SCL_PIN 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Thora smooth reading ke liye 16 samples ka average
int readLdrAverage() {
  long sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += analogRead(LDR_PIN);
    delay(2);
  }
  return sum / 16;
}

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);                 // 0 - 4095
  analogSetPinAttenuation(LDR_PIN, ADC_11db); // poori 0 - 3.3V range

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED nahi mila! SDA/SCL ya address (0x3C/0x3D) check karein");
    while (true) delay(1000);
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int adcValue = readLdrAverage();
  float voltage = (adcValue / 4095.0) * 3.3;
  int barWidth = map(adcValue, 0, 4095, 0, 124);

  display.clearDisplay();

  // Title + line
  display.setTextSize(1);
  display.setCursor(24, 0);
  display.print("LDR MONITOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  // ADC value
  display.setTextSize(1);
  display.setCursor(0, 19);
  display.print("ADC");
  display.setTextSize(2);
  display.setCursor(40, 15);
  display.print(adcValue);

  // Voltage
  display.setTextSize(1);
  display.setCursor(0, 38);
  display.print("Volt");
  display.setTextSize(2);
  display.setCursor(40, 34);
  display.print(voltage, 2);
  display.setTextSize(1);
  display.print(" V");

  // Light level bar
  display.drawRect(0, 54, 128, 10, SSD1306_WHITE);
  display.fillRect(2, 56, barWidth, 6, SSD1306_WHITE);

  display.display();

  Serial.printf("ADC: %d  |  Voltage: %.2f V\n", adcValue, voltage);
  delay(500);
}