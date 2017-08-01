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
bool irFlag = false;

void high();
void low();
void turnLow();
void turnHigh();

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
  pid.init(23, 0.38, 68, 1, 255, MOTOR_LEFT, MOTOR_RIGHT);

}

void loop(){
 LCD.clear();
 LCD.home();
 LCD.print("Waiting to start");
 while(!startbutton());
 delay(1000);
 if(digitalRead(SWITCH_TRACK) == HIGH){
    LCD.clear();
    LCD.home();
    LCD.print("Track 1");
    while(!startbutton());
    high();
 }
 else{
    LCD.clear();
    LCD.home();
    LCD.print("Track 0");
    while(!startbutton());
    low();
 }
 exit(0);
}
void turnLow() {
  motor.speed(MOTOR_LEFT, 200);
  motor.speed(MOTOR_RIGHT, -100);
  
  delay(100);
  LCD.print("flag");
  while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {}  
  while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
    
  circle.lstate=-5;
  pid.run(-5);
}

void low(){
 LCD.clear();
 // drive up to IR gate
 timer = millis();
 //while(millis() < timer + THRESH_TIMER){ 
 count = 0;
 
 while(analogRead(IRPIN_LEFT) < THRESH_IR && analogRead(IRPIN_RIGHT) < THRESH_IR){
      state = pos.get();
      pid.run(state);
      if(count == 10){
        LCD.clear();
        LCD.home();
        LCD.print(analogRead(IRPIN_LEFT));
        LCD.print(" ");
        LCD.print(analogRead(IRPIN_RIGHT));
        count = 0;
      }
      count++;
      if(millis() > timer + THRESH_TIMER){
        irFlag = true;
        LCD.print(" ");
        LCD.print("break 1");
        break;
      }
 }
 pid.stop();
 LCD.print(analogRead(IRPIN_LEFT));
 LCD.print(" ");
 LCD.print(analogRead(IRPIN_RIGHT));
 // wait for IR signal
 while(!irFlag){
  if(analogRead(IRPIN_LEFT) < 100 && analogRead(IRPIN_RIGHT) < 100) {
    LCD.setCursor(1,2);
    LCD.print(analogRead(IRPIN_LEFT));
    LCD.print(" ");
    LCD.print(analogRead(IRPIN_RIGHT));
   
    break;
  }
 } 
  count = 0;
  // drive to tank
  while (analogRead(QRD_OUTER_LEFT) < THRESH_QRD  && analogRead(QRD_OUTER_RIGHT)< THRESH_QRD) {
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

  turnLow();
  LCD.clear();
  LCD.home();
  LCD.print("Done");
  pid.init(20, 0, 50, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);

  //drive to first hashmark
  // might need to do left and right need to test this
  state=-5; //chage dont 5 dont know why
  circle.lstate=state;
  pid.run(state);
  while (analogRead(QRD_RIGHT) < THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  while (analogRead(QRD_RIGHT) > THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  while (analogRead(QRD_RIGHT) < THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  pid.stop();

 LCD.clear();
 LCD.home();
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
         if(/*digitalRead(SWITCH_LIFTUP) == LOW ||*/ digitalRead(SWITCH_LIFTDOWN) == LOW){
           digitalWrite(MOTOR_LIFT_ON, LOW);
           LCD.print("siwtch2");
           break;
         }
      }
      encoderCount++;
      LCD.clear();
      LCD.home();
      LCD.print(encoderCount);

 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 delay(400);

 pid.init(20, 0, 50, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);
 
 while(hashCount < 8){
  state = circle.get();
  if(state > 0){state = 0;}
  if(state == -1) {state = -2;}
  pid.run(state);
  if((analogRead(QRD_RIGHT) > THRESH_QRD || analogRead(QRD_LEFT) > THRESH_QRD) && (analogRead(QRD_CIRCLE_RIGHT) > THRESH_QRD || analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) ){
    delay(50);
    hashCount++;
    LCD.clear();
    LCD.home();
    LCD.print(hashCount);
    state = -1;
    circle.lstate = -1;
  }
 }
 pid.stop();
}



void turnHigh() {
  motor.speed(MOTOR_LEFT, 200);
  motor.speed(MOTOR_RIGHT, -100); // ONLY FOR 1
  //motor.speed(MOTOR_RIGHT, 50); // ONLY FOR 2
  delay(100);
  LCD.print("flag");
  while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {} 
  while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {} 
  while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {}  
  while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
    
  circle.lstate=-5;
  pid.run(-5);
}


void high(){
 LCD.clear();
 LCD.home();

 LCD.clear();
 // drive up to IR gate
 timer = millis();
 //while(millis() < timer + THRESH_TIMER){ 
 count = 0;
 
 while(analogRead(IRPIN_LEFT) < THRESH_IR && analogRead(IRPIN_RIGHT) < THRESH_IR){
      state = pos.get();
      pid.run(state);
      if(count == 10){
        LCD.clear();
        LCD.home();
        LCD.print(analogRead(IRPIN_LEFT));
        LCD.print(" ");
        LCD.print(analogRead(IRPIN_RIGHT));
        count = 0;
      }
      count++;
      if(millis() > timer + THRESH_TIMER){
        irFlag = true;
        LCD.print(" ");
        LCD.print("break 1");
        break;
      }
 }
 pid.stop();
 LCD.print(analogRead(IRPIN_LEFT));
 LCD.print(" ");
 LCD.print(analogRead(IRPIN_RIGHT));
 // wait for IR signal
 while(!irFlag){
  if(analogRead(IRPIN_LEFT) < 100 && analogRead(IRPIN_RIGHT) < 100) {
    LCD.setCursor(1,2);
    LCD.print(analogRead(IRPIN_LEFT));
    LCD.print(" ");
    LCD.print(analogRead(IRPIN_RIGHT));
   
    break;
  }
 } 
  count = 0;
  // drive to tank
  while (analogRead(QRD_OUTER_LEFT) < THRESH_QRD  && analogRead(QRD_OUTER_RIGHT)< THRESH_QRD) {
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

  turnHigh();
  LCD.clear();
  LCD.home();
  LCD.print("Done");
  pid.init(20, 0, 50, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);

  //drive to first hashmark
  // might need to do left and right need to test this
  state=-5; //chage dont 5 dont know why
  circle.lstate=state;
  pid.run(state);
  while (analogRead(QRD_RIGHT) < THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  while (analogRead(QRD_RIGHT) > THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  while (analogRead(QRD_RIGHT) < THRESH_QRD) {
    state = circle.get();
    pid.run(state);
  }
  pid.stop();

 LCD.clear();
 LCD.home();
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
         if(/*digitalRead(SWITCH_LIFTUP) == LOW ||*/ digitalRead(SWITCH_LIFTDOWN) == LOW){
           digitalWrite(MOTOR_LIFT_ON, LOW);
           LCD.print("siwtch2");
           break;
         }
      }
      encoderCount++;
      LCD.clear();
      LCD.home();
      LCD.print(encoderCount);

 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 delay(400);

 pid.init(20, 0, 50, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);
 
 while(hashCount < 8){
  state = circle.get();
  if(state > 0){state = 0;}
  if(state == -1) {state = -2;}
  pid.run(state);
  if((analogRead(QRD_RIGHT) > THRESH_QRD || analogRead(QRD_LEFT) > THRESH_QRD) && (analogRead(QRD_CIRCLE_RIGHT) > THRESH_QRD || analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) ){
    delay(50);
    hashCount++;
    LCD.clear();
    LCD.home();
    LCD.print(hashCount);
    state = -1;
    circle.lstate = -1;
  }
 }
 pid.stop();
}


