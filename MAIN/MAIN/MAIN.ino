//Keypad
#define key1 16 //define keypad pins
#define key2 17 
#define key3 14 
#define key4 15
//LCD
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d11 = 11, d10 = 10, d9 = 9, d8 = 8; //define lcd pins
LiquidCrystal lcd(rs, en, d11, d10, d9, d8); //instantiate lcd 
//SHT
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();
//Neopixel
#include <Adafruit_NeoPixel.h>
#define PIN 6 
#define NUMPIXELS 45 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//Pump
#define out_STBY 3
#define out_A_PWM 7
#define out_A_IN2 4
#define out_A_IN1 5
#define motor_A 0
//Variables
bool power = false;
bool enableHeater = false;
uint8_t loopCnt = 0;
int t = 0;
int h = 0;
bool mode2 = false;
bool mode3 = false;


void setup() {
  //Debug
  Serial.begin(9600);
  //Keypad
  pinMode(key1, INPUT_PULLUP);// set pin as input
  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input   
  //LCD
  lcd.begin(16, 2);// Set up the LCD's number of columns and rows
  lcd.print("Power off");
  //Neopixel
  pixels.begin();
  //SHT
  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");
  //Motor
  pinMode(out_STBY, OUTPUT);
  pinMode(out_A_PWM, OUTPUT);
  pinMode(out_A_IN1, OUTPUT);
  pinMode(out_A_IN2, OUTPUT);
  motor_standby(true);//motor off
  motor_speed(100); // Set speed for motor A
}

void loop() {
  lcd.setCursor(0, 0);
  //Check for key presses
  int key1S = digitalRead(key1);// read if key1 is pressed
  int key2S = digitalRead(key2);// read if key2 is pressed
  int key3S = digitalRead(key3);// read if key3 is pressed
  int key4S = digitalRead(key4);// read if key4 is pressed 
  if(power == false){   //Loop to wait for power on
    if(!key1S){
      power = true;
      lcd.clear();
      lcd.print("Power on");
      delay(1500);
    }
  }else if(power == true){  //Main running loop
    //Mode checks
    if(!key2S){
      example();
    }
    if(!key3S || mode2 == true){
      if(mode2 == false){
        lightOn();
      }
      cycle(2, 60);
      mode3 = false;
      mode2 = true;
    }
    if(!key4S || mode3 == true){
      if(mode3 == false){
        lightOn();
      }
      cycle(3, 80);
      mode2 = false;
      mode3 = true;
    }   
    //Humidity
    t = sht31.readTemperature();
    h = sht31.readHumidity();
    lcd.setCursor(0,1);
    lcd.print("Temp:"); lcd.print(t);
    lcd.setCursor(10,1);
    lcd.print("Hum:"); lcd.print(h);
    if (loopCnt >= 5) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");
    loopCnt = 0;
    }
    loopCnt++;
    //Turn power off
    if(!key1S){
      power = false;
      lcd.clear();
      lcd.print("Power off");
      motor_standby(true);
      pixels.clear();
      pixels.show();
      delay(1500);
    }
  } 
  delay(10);
}

void example() {
  lcd.setCursor(10,0);
  lcd.print("Examp.");
  lightOn();
  motor_standby(false);
  delay(10000);
  pixels.clear();//reset pixels to off state
  pixels.show();//refresh pixels
  motor_standby(true);
}

void cycle(int mode, int humLimit) {
  motor_standby(true);
  lcd.setCursor(10,0);
  lcd.print("Mode ");lcd.print(mode);
  if(h < humLimit){
    motor_standby(false);
  }else{
    motor_standby(true);
  }
}

void lightOn(){
  for(int i=0; i<NUMPIXELS; i+=3) { 
      pixels.setPixelColor(i, pixels.Color(84, 64, 255));
      pixels.setPixelColor(i+1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(i+2, pixels.Color(163, 255, 0));
      pixels.show();  
      delay(5);
  }
}

void motor_speed(char speed) { // Speed from -100 to 100
  byte PWMvalue = map(abs(speed), 0, 100, 50, 255); // Anything below 50 is very weak
  if (speed > 0) {
    digitalWrite(out_A_IN1, HIGH);
    digitalWrite(out_A_IN2, LOW);
  } else if (speed < 0) {
    digitalWrite(out_A_IN1, LOW);
    digitalWrite(out_A_IN2, HIGH);
  } else {
    digitalWrite(out_A_IN1, LOW);
    digitalWrite(out_A_IN2, LOW);
  }
  analogWrite(out_A_PWM, PWMvalue);
}

void motor_standby(boolean state) { // Low power mode
  if (state == true)
    digitalWrite(out_STBY, LOW);
  else
    digitalWrite(out_STBY, HIGH);
}
