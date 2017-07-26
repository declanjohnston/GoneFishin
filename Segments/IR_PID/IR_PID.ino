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
  
}

void loop(){
  // raise arm to zipline
 encoderCount = 0;
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 while(digitalRead(SWITCH_LIFTUP) == HIGH && digitalRead(SWITCH_LIFTDOWN) == HIGH){}

 digitalWrite(MOTOR_LIFT_ON, HIGH);
 
 // drive to zipline
 count = 0;
 while(digitalRead(SWITCH_ZIPLINE) == LOW){

  state = (analogRead(IRPIN_RIGHT) - analogRead(IRPIN_LEFT))/50.0;
  pid.run(state);

  if(count == 10){
    LCD.clear();
    LCD.home();
    LCD.print(state);
    count = 0;
  }
  count++;
  
 }
 pid.stop();
 exit(0);
}

