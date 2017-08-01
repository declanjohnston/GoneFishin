#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>
#include <initialize.h>


void turn();

int state;
int lstate;
bool left;
bool right;
long timer;
int encoderCount = 0;
bool flag;
int hashCount = 0;
int count;
bool irFlag = false;

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
  pid.init(20, 0.30, 55, 1, 255, MOTOR_LEFT, MOTOR_RIGHT);

}


void loop(){
digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 //while(digitalRead(SWITCH_LIFTUP) == LOW){}
 while(encoderCount < ENCODER_RAISEARM){
   //while(true){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){
         if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
            
            LCD.print("a");
            
            break;
        }
      }
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){
        if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
          
          LCD.print("b");
          
          break;
        }
      }
      if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
          if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
            digitalWrite(MOTOR_LIFT_ON, LOW);
            LCD.print("switch");
            LCD.print(digitalRead(SWITCH_LIFTUP));
            LCD.print(" ");
            LCD.print(digitalRead(SWITCH_LIFTDOWN));
            break;
          }
          
      }
      encoderCount++;
      LCD.clear();
      LCD.home();
      LCD.print(encoderCount);
      
 }
 
 digitalWrite(MOTOR_LIFT_ON, LOW);
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 LCD.print(" ");
 LCD.print(encoderCount);

 // turn arm
 encoderCount = 0;
 motor.speed(MOTOR_ARM, 190);
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
 timer = millis();
 while(millis() < timer + 500){
  if(digitalRead(SWITCH_LIFTDOWN) == LOW){
    if(digitalRead(SWITCH_LIFTDOWN) == LOW){
    
   }
  }
 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 delay(400);
 exit(0);
}
