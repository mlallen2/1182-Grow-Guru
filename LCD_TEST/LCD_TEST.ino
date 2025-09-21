#include <LiquidCrystal.h>

// Define the LCD pins
const int rs = 13, en = 12, d11 = 11, d10 = 10, d9 = 9, d8 = 8;
LiquidCrystal lcd(rs, en, d11, d10, d9, d8);

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Hello World!");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
}

void loop() {
  // No need for anything in the loop for this example.
}
