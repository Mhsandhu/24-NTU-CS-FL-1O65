// MUHAMMAD HASNAIN ANJUM 

// 24-NTU-CS-FL-2026

// GRAPHICAL VIEW

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C
#define DHT_PIN 18

// Real hardware = DHT11 (default). Sirf Wokwi build me DHT22 use hota hai.
#ifdef WOKWI_SIM
  #define DHT_TYPE DHT22
#else
  #define DHT_TYPE DHT11
#endif

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHT_PIN, DHT_TYPE);

const unsigned long READ_INTERVAL_MS = 2000;
unsigned long lastReadTime = 0;
bool firstRead = true;

// ---------------- Graph layout (128x64) ----------------
const int AXIS_X     = 20;   // vertical Y-axis line
const int BASE_Y     = 53;   // y position of value 0
const int TOP_Y      = 3;    // y position of value 100
const int PLOT_H     = BASE_Y - TOP_Y;  // 50 px for 0..100
const int BAR_W      = 14;
const int TEMP_BAR_X = 24;   // temperature bar
const int HUM_BAR_X  = 80;   // humidity bar
const int LABEL_Y    = 56;   // "TEMP" / "HUMIDITY" labels

// Value (0..100) ko pixel height me convert karta hai
int valueToHeight(float value) {
  value = constrain(value, 0.0f, 100.0f);
  return (int)(value * PLOT_H / 100.0f + 0.5f);
}

void showMessage(const char* line1, const char* line2) {
  int16_t x1, y1;
  uint16_t w, h;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.getTextBounds(line1, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 22);
  display.print(line1);

  display.getTextBounds(line2, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 36);
  display.print(line2);

  display.display();
}

// Dotted "track" jo bar ki full height (0..100) dikhata hai
void drawTrack(int x) {
  for (int y = TOP_Y; y <= BASE_Y; y += 3) {
    display.drawPixel(x, y, SSD1306_WHITE);
    display.drawPixel(x + BAR_W - 1, y, SSD1306_WHITE);
  }
  display.drawFastHLine(x, TOP_Y, BAR_W, SSD1306_WHITE);
}

void drawBar(int x, float value) {
  drawTrack(x);
  const int h = valueToHeight(value);
  if (h > 0) {
    display.fillRect(x, BASE_Y - h, BAR_W, h, SSD1306_WHITE);
  }
}

// Bar ke saath value ka text (bar ki height ke hisab se upar-neeche)
void drawValue(int x, float value, const char* text) {
  const int barTop = BASE_Y - valueToHeight(value);
  const int y = constrain(barTop - 3, 0, 44);
  display.setCursor(x, y);
  display.print(text);
}

void drawAxes() {
  // Y-axis aur X-axis
  display.drawFastVLine(AXIS_X, TOP_Y, PLOT_H + 1, SSD1306_WHITE);
  display.drawFastHLine(AXIS_X, BASE_Y, SCREEN_WIDTH - AXIS_X, SSD1306_WHITE);

  // Tick marks har 20 unit par (0,20,40,60,80,100)
  for (int v = 0; v <= 100; v += 20) {
    const int y = BASE_Y - (v * PLOT_H) / 100;
    display.drawFastHLine(AXIS_X - 3, y, 3, SSD1306_WHITE);
  }

  // Y-axis labels
  display.setTextSize(1);
  display.setCursor(0, TOP_Y - 3);
  display.print("100");
  display.setCursor(6, BASE_Y - PLOT_H / 2 - 3);
  display.print("50");
  display.setCursor(12, BASE_Y - 3);
  display.print("0");
}

void showMeasurements(float humidity, float temperature) {
  char buf[16];

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  drawAxes();

  // Bars
  drawBar(TEMP_BAR_X, temperature);
  drawBar(HUM_BAR_X, humidity);

  // Value text
  snprintf(buf, sizeof(buf), "%.1f", temperature);
  display.setCursor(TEMP_BAR_X + BAR_W + 3, constrain(BASE_Y - valueToHeight(temperature) - 3, 0, 44));
  display.print(buf);
  display.write(247);  // degree symbol
  display.print("C");

  snprintf(buf, sizeof(buf), "%.1f%%", humidity);
  drawValue(HUM_BAR_X + BAR_W + 3, humidity, buf);

  // Bottom labels
  display.setCursor(22, LABEL_Y);
  display.print("TEMP");
  display.setCursor(78, LABEL_Y);
  display.print("HUMIDITY");

  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("ERROR: OLED initialization failed. Check power, SDA/SCL, and I2C address.");
    while (true) { delay(100); }
  }

  dht.begin();
  showMessage("ESP32 ENVIRONMENT", "Starting sensor...");
  Serial.print("ESP32 monitor started. Sensor type: ");
  Serial.println(DHT_TYPE == DHT11 ? "DHT11" : "DHT22");
  delay(1500);  // sensor ko stable hone ka time
}

void loop() {
  const unsigned long now = millis();
  if (firstRead || (now - lastReadTime >= READ_INTERVAL_MS)) {
    firstRead = false;
    lastReadTime = now;

    const float humidity = dht.readHumidity();
    const float temperature = dht.readTemperature();

    // NaN ya impossible values ko reject karo (galat sensor type / noise)
    const bool invalid = isnan(humidity) || isnan(temperature) ||
                         humidity < 0 || humidity > 100 ||
                         temperature < -40 || temperature > 80;

    if (invalid) {
      Serial.println("DHT read failed or out of range! Check DATA pin, power, GND, sensor type.");
      showMessage("SENSOR ERROR", "Check DHT wiring");
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity, 1);
    Serial.print(" %   Temperature: ");
    Serial.print(temperature, 1);
    Serial.println(" C");
    showMeasurements(humidity, temperature);
  }
}