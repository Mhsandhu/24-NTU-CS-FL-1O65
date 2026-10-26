// #include <Arduino.h>

// #define BUZZER_PIN 18
// #define PWM_CHANNEL 0
// #define PWM_RESOLUTION 8
// #define PWM_INITIAL_FREQ 1000

// // Generate a tone
// void playTone(int frequency, int duration) {
//   ledcWriteTone(PWM_CHANNEL, frequency);
//   delay(duration);

//   ledcWriteTone(PWM_CHANNEL, 0);
//   delay(100);
// }

// // Pattern 1: Rising tones
// void toneSequence() {
//   playTone(300, 500);
//   playTone(600, 500);
//   playTone(1000, 500);
//   playTone(1500, 500);
//   playTone(2000, 500);
// }

// // Pattern 2: Alarm
// void alarmSound() {
//   for (int i = 0; i < 5; i++) {
//     playTone(1000, 300);
//     playTone(2000, 300);
//   }
// }

// // Pattern 3: Doorbell
// void doorbell() {
//   playTone(1000, 300);
//   playTone(1500, 600);
//   delay(300);
//   playTone(1000, 300);
//   playTone(1500, 600);
// }

// // Pattern 4: Short beeps
// void shortBeeps() {
//   for (int i = 0; i < 5; i++) {
//     playTone(2000, 100);
//     delay(100);
//   }
// }

// // Pattern 5: Simple melody
// void melody() {
//   playTone(523, 300);   // C5
//   playTone(587, 300);   // D5
//   playTone(659, 300);   // E5
//   playTone(698, 300);   // F5
//   playTone(784, 300);   // G5
//   playTone(880, 300);   // A5
//   playTone(988, 300);   // B5
//   playTone(1047, 600);  // C6
// }

// void setup() {
//   Serial.begin(115200);

//   // Configure PWM using the old API
//   ledcSetup(PWM_CHANNEL, PWM_INITIAL_FREQ, PWM_RESOLUTION);

//   // Attach PWM channel to GPIO 16
//   ledcAttachPin(BUZZER_PIN, PWM_CHANNEL);

//   Serial.println("PWM Buzzer Ready!");
// }

// void loop() {
//   Serial.println("1. Rising Tone Sequence");
//   toneSequence();
//   delay(500);

//   Serial.println("2. Alarm");
//   alarmSound();
//   delay(500);

//   Serial.println("3. Doorbell");
//   doorbell();
//   delay(500);

//   Serial.println("4. Short Beeps");
//   shortBeeps();
//   delay(500);

//   Serial.println("5. Melody");
//   melody();
//   delay(2000);
// }





#include <Arduino.h>

#define BUZZER_PIN 18
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8
#define PWM_BASE_FREQ 2000

// Musical notes: frequencies in Hz
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784

// Happy Birthday melody
const int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_B4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_G4, NOTE_G4, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5
};

// Note durations in milliseconds
const int durations[] = {
  300, 300, 600, 600, 600, 900,
  300, 300, 600, 600, 600, 900,
  300, 300, 600, 600, 600, 600, 900,
  300, 300, 600, 600, 600, 1200
};

const int TOTAL_NOTES = sizeof(melody) / sizeof(melody[0]);

void playNote(int frequency, int duration) {
  ledcWriteTone(PWM_CHANNEL, frequency);
  delay(duration);

  ledcWriteTone(PWM_CHANNEL, 0);
  delay(70);
}

void playHappyBirthday() {
  for (int i = 0; i < TOTAL_NOTES; i++) {
    playNote(melody[i], durations[i]);
  }
}

void setup() {
  Serial.begin(115200);

  // Older ESP32 Arduino core LEDC API
  ledcSetup(PWM_CHANNEL, PWM_BASE_FREQ, PWM_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CHANNEL);

  Serial.println("Happy Birthday Music Player Ready!");
  delay(1000);
}

void loop() {
  Serial.println("Playing Happy Birthday!");
  playHappyBirthday();

  Serial.println("Song finished. Replaying in 3 seconds...");
  delay(3000);
}
