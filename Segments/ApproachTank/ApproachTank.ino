#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>
#include <initialize.h>




int state;
int lstate;
bool left;
bool right;
int timer;
int encoderCount = 0;
bool flag;
int hashCount = 0;
int count;

PID pid = PID(motor);
State pos = State(QRD_LEFT, QRD_RIGHT, THRESH_QRD, QRD_CIRCLE_LEFT);
State circle = State(QRD_CIRCLE_LEFT, QRD_CIRCLE_RIGHT, THRESH_QRD, QRD_RIGHT);

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  initPins();
  // remember to initialize values
  // derivative, integral, proportional,gain, speed
  pid.init(10, 0.2, 35, 1, 200, MOTOR_LEFT, MOTOR_RIGHT);

}

void loop() {
  count = 0;
  // drive to tank
  while ((analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD  && analogRead(QRD_OUTER_RIGHT)< THRESH_QRD) || (analogRead(QRD_LEFT) < THRESH_QRD && analogRead(QRD_RIGHT) < THRESH_QRD)) {
    state = pos.get();
    pid.run(state);
    if (count == 10) {
      LCD.clear();
      LCD.home();
      LCD.print(analogRead(QRD_RIGHT));
      LCD.print(" ");
      LCD.print(analogRead(QRD_LEFT));
      LCD.print(" ");
      LCD.print(analogRead(QRD_OUTER_RIGHT));
      count = 0;
    }
    count++;
  }
  pid.stop();
  delay(400); //stops before it turns

  // turn corner
  LCD.clear();
  LCD.home();
  LCD.print("Turning");

  turn();
  LCD.clear();
  LCD.home();
  LCD.print("Done");
  pid.init(20, 0, 50, 1, 160, MOTOR_LEFT, MOTOR_RIGHT);

  //drive to first hashmark
  // might need to do left and right need to test this
  state=-5; //chage dont 5 dont know why
  circle.lstate=state;
  pid.run(state);
  while (analogRead(QRD_RIGHT) < THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  pid.stop();
    LCD.print("!");
  exit(0);
}

void turn() {
  motor.speed(MOTOR_LEFT, 200);
  motor.speed(MOTOR_RIGHT, 50);
  delay(100);
  //while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
  LCD.print("flag");
  while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {}
    LCD.print("flag2");
  while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
    LCD.print("flag3");
  circle.lstate=-5;
  pid.run(-5);
}

