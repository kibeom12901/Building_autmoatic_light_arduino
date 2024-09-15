#include <LiquidCrystal.h>  // Standard library for parallel LCD

// Initialize the LCD with the corresponding pins
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);  // LCD pins: RS=6, E=7, D4=5, D5=4, D6=3, D7=2

const int sampleWindow = 100;  // Sample window width in ms (100 ms = 10Hz)
unsigned int sample;

#define SENSOR_PIN A0         // Sound sensor connected to A0
#define LED_PIN_1 8           // First LED connected to digital pin 8
#define LED_PIN_2 9           // Second LED connected to digital pin 9

int ledState1 = LOW;          // Start with LED 1 off (LOW)
int ledState2 = LOW;          // Start with LED 2 off (LOW)
bool ledOn = false;           // Track if LEDs are currently on or off
unsigned long alternateEndTime = 0;  // Time for alternating LEDs

void setup() {
  pinMode(SENSOR_PIN, INPUT);   // Set the sound sensor pin as input
  pinMode(LED_PIN_1, OUTPUT);   // Set LED 1 pin as output
  pinMode(LED_PIN_2, OUTPUT);   // Set LED 2 pin as output

  digitalWrite(LED_PIN_1, LOW);  // Initialize LED 1 to off
  digitalWrite(LED_PIN_2, LOW);  // Initialize LED 2 to off
  Serial.begin(115200);

  // Initialize the LCD
  lcd.begin(16, 2);  // Initialize LCD for 16x2
  lcd.clear();       // Clear the LCD
  lcd.setCursor(0, 0);
  lcd.print("Clap to turn on");  // Initial message
}

void loop() {
  unsigned long startMillis = millis();  // Start of sample window
  float peakToPeak = 0;  // Peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // Collect data for 100 ms
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SENSOR_PIN);  // Get reading from sound sensor
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

  // Clap detection logic: If loudness exceeds 70 dB (loud clap), start alternating the LEDs
  if (db >= 70 && !ledOn) {
    delay(100);  // Debounce delay to avoid multiple triggers for one clap

    // Start alternating the LEDs
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alternating LEDs");  // Display message on LCD

    ledOn = true;  // Set the flag that LEDs have been turned on
    alternateEndTime = millis() + 7000;  // Set the total time for alternating LEDs (7 seconds)
  }

  // If the LEDs are on, alternate them every 0.5 seconds for 7 seconds
  if (ledOn && millis() < alternateEndTime) {
    if (millis() % 1000 < 500) {
      digitalWrite(LED_PIN_1, HIGH);  // Turn LED 1 on
      digitalWrite(LED_PIN_2, LOW);   // Turn LED 2 off
    } else {
      digitalWrite(LED_PIN_1, LOW);   // Turn LED 1 off
      digitalWrite(LED_PIN_2, HIGH);  // Turn LED 2 on
    }
  }

  // After 7 seconds, stop alternating and reset both LEDs to off
  if (ledOn && millis() >= alternateEndTime) {
    ledOn = false;  // Reset the flag
    digitalWrite(LED_PIN_1, LOW);  // Turn off LED 1
    digitalWrite(LED_PIN_2, LOW);  // Turn off LED 2
    lcd.clear();  // Clear the display
    lcd.setCursor(0, 0);
    lcd.print("Clap to turn on");  // Reset message to "Clap to turn on"
  }

  delay(100);  // Short delay between readings
}
