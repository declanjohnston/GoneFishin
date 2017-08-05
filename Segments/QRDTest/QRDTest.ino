#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>
#include <initialize.h>
#include <Bounce2.h>




int state;
int lstate;
bool left;
bool right;
int timer;
int encoderCount = 0;
bool flag;
int hashCount = 0;
int count;
Bounce BounceLiftUp = Bounce();
Bounce BounceLiftDown = Bounce();
Bounce BounceSwitchZipline = Bounce();

PID pid=PID(motor);
State pos=State(QRD_LEFT, QRD_RIGHT,THRESH_QRD, QRD_CIRCLE_LEFT);
State circle=State(QRD_CIRCLE_LEFT, QRD_CIRCLE_RIGHT, THRESH_QRD, QRD_RIGHT);

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  initPins();
  // remember to initialize values
  // derivative, integral, proportional,gain, speed
  pid.init(20,0,35,1,255, MOTOR_LEFT, MOTOR_RIGHT);
  BounceLiftUp.interval(3);
  BounceLiftUp.attach(SWITCH_LIFTUP);
  BounceLiftDown.interval(3);
  BounceLiftDown.attach(SWITCH_LIFTDOWN);
  BounceSwitchZipline.interval(3);
  BounceSwitchZipline.attach(SWITCH_ZIPLINE);
}
void loop() {
  // put your main code here, to run repeatedly:

  LCD.clear();
  LCD.home();
  LCD.print(analogRead(QRD_CIRCLE_RIGHT));
  LCD.print(" ");
  LCD.print(analogRead(QRD_CIRCLE_LEFT));
  delay(300);

}
