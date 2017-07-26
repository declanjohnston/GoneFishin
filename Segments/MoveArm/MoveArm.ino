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
  //raise arm
 LCD.clear();
 LCD.home();
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 while(encoderCount < ENCODER_RAISEARM&&(digitalRead(SWITCH_LIFTUP) != LOW /*&& digitalRead(SWITCH_LIFTDOWN) != LOW*/)){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){
         if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
        LCD.print("switch");
        break;
        }
      }
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){
        if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
        LCD.print("switch");
        break;
        }
      }
        if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
        LCD.print("switch");
        break;
        }
      encoderCount++;
      LCD.clear();
      LCD.home();
      LCD.print(encoderCount);
      
 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 

 // turn arm
 encoderCount = 0;
 motor.speed(MOTOR_ARM, 200);
 //Arm is switches not encoder !!! Needs to change
 while(digitalRead(SWITCH_ARM_1) == LOW){
//    if(digitalRead(SWITCH_ARM_2) == HIGH){
//      break;
//    }
 }
 motor.speed(MOTOR_ARM, 0);

 // lower arm
 encoderCount = 0;
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 while(encoderCount < ENCODER_LOWERARM){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){
        if(/*digitalRead(SWITCH_LIFTUP) == LOW ||*/ digitalRead(SWITCH_LIFTDOWN) == LOW){
          break;
        }
      }
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){
        if(/*digitalRead(SWITCH_LIFTUP) == LOW ||*/ digitalRead(SWITCH_LIFTDOWN) == LOW){
          break;
        }
      }
      if(/*digitalRead(SWITCH_LIFTUP) == LOW ||*/ digitalRead(SWITCH_LIFTDOWN) == LOW){
        break;
      }
      encoderCount++;
      LCD.clear();
      LCD.home();
      LCD.print(encoderCount);

 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 exit(0);
}

