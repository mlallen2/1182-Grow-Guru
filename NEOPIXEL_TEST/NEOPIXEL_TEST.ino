#include <Adafruit_NeoPixel.h>

#define LED_PIN    6 // Pin connected to the NeoPixels
#define LED_COUNT  45 // Number of NeoPixels in the ring

// Initialize NeoPixel strip object
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize NeoPixel strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Set all LEDs to white
  colorWipe(strip.Color(255, 255, 255), 50); // Change the second parameter to adjust the transition speed

  // Set all LEDs to purple
  colorWipe(strip.Color(128, 0, 128), 50); // Change the second parameter to adjust the transition speed
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}
