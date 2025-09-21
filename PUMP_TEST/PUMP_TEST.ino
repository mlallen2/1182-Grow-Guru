#define out_STBY 3
#define out_A_PWM 7
#define out_A_IN2 4
#define out_A_IN1 5
#define motor_A 0

void setup()
{
  pinMode(out_STBY, OUTPUT);
  pinMode(out_A_PWM, OUTPUT);
  pinMode(out_A_IN1, OUTPUT);
  pinMode(out_A_IN2, OUTPUT);
  motor_standby(false);
  motor_speed(100); // Set speed for motor A
  delay(2000);
  motor_standby(true);
}

void loop()
{

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