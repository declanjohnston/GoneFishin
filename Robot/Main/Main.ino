#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>



byte MOTOR_LEFT = 0;
byte MOTOR_RIGHT = 1;
byte MOTOR_LIFT = 2;
byte MOTOR_ARM = 3; 
int THRESH_IR;
int THRESH_TIMER;
int THRESH_QRD;
byte IRPIN_LEFT;
byte IRPIN_RIGHT;
byte QRD_LEFT_OUT;
byte QRD_RIGHT_OUT;
byte QRD_LEFT;
byte QRD_RIGHT;
int ENCODER_TURN;
int ENCODER_RAISEARM;
int ENCODER_LOWERARM;
int ENCODER_ZIPLINE;
int ENCODER_LIFTPIN;
int ENCODER_ARMPIN;
int ENCODER_FINAL;
byte TOTAL_HASHES;
byte SWITCH_ZIPLINE;

int state;
int lstate;
bool left;
bool right;
int timer;
int encoderCount = 0;
bool flag;
int hashCount = 0;

PID pid=PID(motor);
State pos=State(QRD_LEFT, QRD_RIGHT,THRESH_QRD);

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;

  pid.init(0,0,0,0,0, MOTOR_LEFT, MOTOR_RIGHT);
}

void loop()
{
 // drive up to IR gate
 timer = millis();
 while(millis() < timer + THRESH_TIMER){ 
    
      state = pos.get();
      pid.run(state);
      
 }
 pid.stop();
 
 // wait for IR signal
 while(analogRead(IRPIN_LEFT) < THRESH_IR){} 

 // drive to tank
 while(analogRead(QRD_RIGHT_OUT) < THRESH_QRD){ 
      state = pos.get();
      pid.run(state);
 }
 pid.stop();

 // turn corner
 turn(); 

 //drive to first hashmark
 while(analogRead(QRD_LEFT_OUT) < THRESH_QRD){ 
      state = pos.get();
      pid.run(state);
 }
 pid.stop();
 
 //raise arm
 motor.speed(MOTOR_LIFT, 255);
 while(encoderCount < ENCODER_RAISEARM){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){}
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){}
      encoderCount++;
 }
 motor.speed(MOTOR_LIFT,0);

 // turn arm
 encoderCount = 0;
 motor.speed(MOTOR_ARM, 255);
 while(encoderCount < ENCODER_TURN){
      while(digitalRead(ENCODER_ARMPIN) == LOW){}
      while(digitalRead(ENCODER_ARMPIN) == HIGH){}
      encoderCount++;
 }
 motor.speed(MOTOR_ARM, 0);

 // lower arm
 encoderCount = 0;
 motor.speed(MOTOR_LIFT, -255);
 while(encoderCount < ENCODER_LOWERARM){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){}
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){}
      encoderCount++;
 }
 motor.speed(MOTOR_LIFT, 0);

 // drive around circle
 flag = false;
 while(hashCount < TOTAL_HASHES){
      state = pos.get();
      pid.run(state);
      if(flag){
        if(analogRead(QRD_RIGHT_OUT) < THRESH_QRD){
          flag = false;
          hashCount++;
        }
      }
      else if(analogRead(QRD_RIGHT_OUT) > THRESH_QRD){
        flag = true;
      }
 }
 pid.stop();

 // raise arm to zipline
 encoderCount = 0;
 motor.speed(MOTOR_LIFT, 255);
 while(encoderCount < ENCODER_ZIPLINE - ENCODER_RAISEARM + ENCODER_LOWERARM){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){}
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){}
      encoderCount++;
 }
 motor.speed(MOTOR_LIFT,0);

 // drive to zipline
 while(digitalRead(SWITCH_ZIPLINE) == LOW){
  state = analogRead(IRPIN_RIGHT) - analogRead(IRPIN_LEFT);
  pid.run(state);
 }
 pid.stop();

 // raise body
 encoderCount = 0;
 motor.speed(MOTOR_LIFT, -255);
 while(encoderCount < ENCODER_FINAL){
      while(digitalRead(ENCODER_LIFTPIN) == LOW){}
      while(digitalRead(ENCODER_LIFTPIN) == HIGH){}
      encoderCount++;
 }
 motor.speed(MOTOR_LIFT, 0);
}

void turn(){
  motor.speed(MOTOR_LEFT, 0);
  motor.speed(MOTOR_RIGHT, 255);
  while(analogRead(QRD_RIGHT_OUT) > THRESH_QRD){}
  while(analogRead(QRD_RIGHT_OUT) < THRESH_QRD){}
  pid.run(-5);
}

