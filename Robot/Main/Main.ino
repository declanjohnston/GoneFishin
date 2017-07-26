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

void loop()
{
 LCD.clear();
 LCD.home();
 LCD.print("Waiting to start");
 while(!startbutton());
 LCD.clear();
 // drive up to IR gate
 //timer = millis();
 //while(millis() < timer + THRESH_TIMER){ 
 count = 0;
 while(analogRead(IRPIN_LEFT) < THRESH_IR && analogRead(IRPIN_RIGHT) < THRESH_IR){
      state = pos.get();
      pid.run(state);
      if(count == 50){
        LCD.clear();
        LCD.home();
        LCD.print(analogRead(IRPIN_LEFT));
        LCD.print(" ");
        LCD.print(analogRead(IRPIN_RIGHT));
        count = 0;
      }
      count++;
 }
 pid.stop();
 
 // wait for IR signal
 while(analogRead(IRPIN_LEFT) > THRESH_IR || analogRead(IRPIN_RIGHT) > THRESH_IR){} 

 // drive to tank
 while(analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD || (analogRead(QRD_LEFT) < THRESH_QRD && analogRead(QRD_RIGHT) < THRESH_QRD)) { 
      state = pos.get();
      pid.run(state);
 }
 pid.stop();
 delay(400); //stops before it turns

 pid.init(10,0,25,1,160, MOTOR_LEFT, MOTOR_RIGHT);
 // turn corner
 turn(); 

 //drive to first hashmark
 // might need to do left and right need to test this
 state=-5; //chage dont 5 dont know why
 circle.lstate=state;
 pid.run(state);
 while(analogRead(QRD_RIGHT) < THRESH_QRD){ 
      state = circle.tank();
      pid.run(state);
 }
 pid.stop();
 
 //raise arm
LCD.clear();
 LCD.home();
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 while(encoderCount < ENCODER_RAISEARM&&(digitalRead(SWITCH_LIFTUP) != LOW && digitalRead(SWITCH_LIFTDOWN) != LOW)){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){
         if(digitalRead(SWITCH_LIFTUP) == LOW || digitalRead(SWITCH_LIFTDOWN) == LOW){
        LCD.print("switch");
        break;
        }
      }
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){
        if(digitalRead(SWITCH_LIFTUP) == LOW || digitalRead(SWITCH_LIFTDOWN) == LOW){
        LCD.print("switch");
        break;
        }
      }
        if(digitalRead(SWITCH_LIFTUP) == LOW || digitalRead(SWITCH_LIFTDOWN) == LOW){
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
    if(digitalRead(SWITCH_ARM_2) == HIGH){
      break;
    }
 }
 motor.speed(MOTOR_ARM, 0);

 //lower arm
 encoderCount = 0;
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 while(encoderCount < ENCODER_LOWERARM){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){
        if(digitalRead(SWITCH_LIFTUP) == LOW || digitalRead(SWITCH_LIFTUP) == LOW){
          break;
        }
      }
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){
        if(digitalRead(SWITCH_LIFTUP) == LOW || digitalRead(SWITCH_LIFTUP) == LOW){
          break;
        }
      }
      if(digitalRead(SWITCH_LIFTUP) == LOW || digitalRead(SWITCH_LIFTUP) == LOW){
        break;
      }
      encoderCount++;
      LCD.clear();
      LCD.home();
      LCD.print(encoderCount);

 }
 // drive around circle
 flag = false;
 while(hashCount < TOTAL_HASHES){
      state = circle.straddle();
      pid.run(state);
      if(flag){
        if(!circle.out){ // again might want ti make it right and left not jut one
          flag = false;
          hashCount++;
        }
      }
      else if(circle.out){
        flag = true;
      }
 }
 pid.stop();

 // raise arm to zipline
 encoderCount = 0;
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 while(digitalRead(SWITCH_LIFTUP) == HIGH && digitalRead(SWITCH_LIFTDOWN) == HIGH){}

 digitalWrite(MOTOR_LIFT_ON, LOW);
 
 // drive to zipline
 while(digitalRead(SWITCH_ZIPLINE) == LOW){
  state = (analogRead(IRPIN_RIGHT) - analogRead(IRPIN_LEFT))/50.0;
  pid.run(state);
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

void turn(){
  motor.speed(MOTOR_LEFT, 200);
  motor.speed(MOTOR_RIGHT, 50);
  delay(100);
 // while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
  
  while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {}
    
  while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
    
  circle.lstate=-5;
  pid.run(-5);
}

