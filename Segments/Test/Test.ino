#include  <phys253.h>
#include  <LiquidCrystal.h>
#include  <initialize.h>
#include  <Bounce2.h>

Bounce encoderBounce = Bounce();
Bounce LiftUpBounce = Bounce();
Bounce LiftDownBounce = Bounce();
int encoderCount = 0;


void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  initPins();
  // remember to initialize values
  // derivative, integral, proportional,gain, speed
  encoderBounce.interval(3);
  encoderBounce.attach(ENCODER_LIFTPIN);
  LiftUpBounce.interval(3);
  LiftDownBounce.interval(3);
  LiftUpBounce.attach(SWITCH_LIFTUP);
  LiftDownBounce.attach(SWITCH_LIFTDOWN);
}

void loop() { 
//  // put your main code here, to run repeatedly:
//  LCD.clear();
//  LCD.home();
//LCD.print(analogRead(0));
//LCD.print(" ");
//LCD.print(analogRead(1));
//
//  delay(100);
   digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
   digitalWrite(MOTOR_LIFT_ON, HIGH);
   
  LiftUpBounce.update();
  while(!LiftUpBounce.read()){
    LiftUpBounce.update();
  }
// while(encoderCount < ENCODER_RAISEARM){
//   //while(true){
//      while(digitalRead(ENCODER_LIFTPIN) == LOW){
//         if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
//            
//            LCD.print("a");
//            
//            break;
//        }
//      }
//      while(digitalRead(ENCODER_LIFTPIN) == HIGH){
//        if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
//          
//          LCD.print("b");
//          
//          break;
//        }
//      }
//      if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
//          if(digitalRead(SWITCH_LIFTUP) == LOW /*|| digitalRead(SWITCH_LIFTDOWN) == LOW*/){
//            digitalWrite(MOTOR_LIFT_ON, LOW);
//            LCD.print("switch");
//            LCD.print(digitalRead(SWITCH_LIFTUP));
//            LCD.print(" ");
//            LCD.print(digitalRead(SWITCH_LIFTDOWN));
//            break;
//          }
//          
//      }
//      encoderCount++;
//      LCD.clear();
//      LCD.home();
//      LCD.print(encoderCount);
//      
// }
 
 digitalWrite(MOTOR_LIFT_ON, LOW);
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 LCD.print(" ");
 LCD.print("a");

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
 LCD.print(" ");
 LCD.print("b");
 
 
 LiftDownBounce.attach(SWITCH_LIFTDOWN);
 LiftDownBounce.update();
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 
 while(encoderCount < ENCODER_LOWERARM && !LiftDownBounce.read()){
    encoderBounce.update();
    if(encoderBounce.rose()){
      encoderCount++;
      LCD.clear();
      LCD.print(encoderCount);
    }
    LiftDownBounce.update();
 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 delay(400);
 LCD.print(" ");
 LCD.print("c");
 exit(0);
}


