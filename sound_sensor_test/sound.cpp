#include <LiquidCrystal.h>  // Standard library for parallel LCD

// Initialize the LCD with the corresponding pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sampleWindow = 50;  // Sample window width in ms (50 ms = 20Hz)
unsigned int sample;

#define SENSOR_PIN A0
#define PIN_QUIET 3
#define PIN_MODERATE 4
#define PIN_LOUD 5

void setup() {
  pinMode(SENSOR_PIN, INPUT);  // Set the signal pin as input
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT);

  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);

  Serial.begin(115200);
  lcd.begin(16, 2);  // Initialize LCD for 16x2
  lcd.clear();       // Clear the LCD
}

void loop() {
  unsigned long startMillis = millis();  // Start of sample window
  float peakToPeak = 0;  // Peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // Collect data for 50 ms
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SENSOR_PIN);  // Get reading from microphone
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;  // Save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // Save just the min levels
      }
    }
  }

  peakToPeak = signalMax - signalMin;  // Max - min = peak-peak amplitude
  int db = map(peakToPeak, 20, 900, 49, 90);  // Calibrate for decibels

  lcd.setCursor(0, 0);
  lcd.print("Loudness: ");
  lcd.print(db);
  lcd.print("dB");

  // Sound level logic
  if (db <= 60) {
    lcd.setCursor(0, 1);
    lcd.print("Level: Quiet  ");
    digitalWrite(PIN_QUIET, HIGH);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
  } else if (db > 60 && db < 85) {
    lcd.setCursor(0, 1);
    lcd.print("Level: Moderate");
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, HIGH);
    digitalWrite(PIN_LOUD, LOW);
  } else if (db >= 85) {
    lcd.setCursor(0, 1);
    lcd.print("Level: High   ");
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, HIGH);
  }

  delay(200);
  lcd.clear();
}
