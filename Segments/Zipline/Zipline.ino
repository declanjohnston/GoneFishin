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
Bounce LiftUpBounce = Bounce();

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
  LiftUpBounce.interval(3);
  LiftUpBounce.attach(SWITCH_LIFTUP);
}
void loop() {
   // raise arm to zipline
 flag = false;
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 LiftUpBounce.update();
 while(LiftUpBounce.read()){
  LiftUpBounce.update();
 }

 digitalWrite(MOTOR_LIFT_ON, LOW);
 pid.init(0,1,35,1,255, MOTOR_LEFT, MOTOR_RIGHT);
  
 // drive to zipline
// while(digitalRead(SWITCH_ZIPLINE) == LOW){
//  state = (analogRead(IRPIN_RIGHT) - analogRead(IRPIN_LEFT))/50.0;
//  pid.run(state);
// }
 motor.speed(MOTOR_RIGHT,140);
 motor.speed(MOTOR_LEFT, 110);
 while(digitalRead(SWITCH_ZIPLINE) == LOW){
 }
 pid.stop();

 // raise body

 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH); //  change for 30V


 while(true){
  while(digitalRead(SWITCH_LIFTDOWN) == HIGH){
    digitalWrite(MOTOR_LIFT_ON, HIGH);
    
  }
  digitalWrite(MOTOR_LIFT_ON, LOW);
  
 }

}
