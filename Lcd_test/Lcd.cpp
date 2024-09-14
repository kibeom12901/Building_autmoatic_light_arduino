#include <LiquidCrystal.h>  // Standard LCD library for Arduino

// Initialize the LCD with the corresponding pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);  // Start the LCD and set its dimensions (16 columns and 2 rows)
  
  // Print a simple message
  lcd.setCursor(0, 0);  // Set cursor to the first column and first row
  lcd.print("Hello, World!");  // Print the message
  delay(2000);  // Wait for 2 seconds
  
  lcd.setCursor(0, 1);  // Move to the second row
  lcd.print("LCD Test Done!");  // Print a second message
}

void loop() {
  // Nothing needed here for this test
}
