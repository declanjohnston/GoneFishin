#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>
#include <initialize.h>
#include <Bounce2.h>


void turn();

int state;
int lstate;
bool left;
bool right;
long timer;

bool flag;
int hashCount = 0;
int count;
bool irFlag = false;
Bounce BounceLiftMid = Bounce();
Bounce BounceLiftUp = Bounce();
Bounce BounceLiftDown = Bounce();
Bounce BounceSwitchZipline = Bounce();
Bounce BounceSwitchArm = Bounce();
Bounce BounceSwitchArm2 = Bounce();
Bounce BounceSwitchSide = Bounce();

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
  pid.init(25, 0, 55, 1, 255, MOTOR_LEFT, MOTOR_RIGHT);
  BounceLiftMid.interval(3);
  BounceLiftUp.interval(3);
  BounceLiftDown.interval(3);
  BounceLiftUp.attach(SWITCH_LIFTUP);
  BounceLiftDown.attach(SWITCH_LIFTDOWN);
  BounceSwitchZipline.interval(3);
  BounceSwitchArm.interval(3);
  BounceSwitchArm2.interval(3);
  BounceSwitchSide.interval(3);
  BounceSwitchSide.attach(SWITCH_SIDE);
}

void loop() {
 LCD.clear();
 LCD.home();
 LCD.print("Waiting to start");
 while(!startbutton());
 delay(500);
 LCD.clear();
 // drive up to IR gate
 timer = millis();
 //while(millis() < timer + THRESH_TIMER){ 
 while(millis() < timer + THRESH_TIMER){ 
  state = pos.get();
  pid.run(state);
 }
 pid.stop();
 count = 0;
 while(analogRead(IRPIN_LEFT) < THRESH_IR && analogRead(IRPIN_RIGHT) < THRESH_IR){
      if(count == 10){
        LCD.clear();
        LCD.home();
        LCD.print(analogRead(IRPIN_LEFT));
        LCD.print(" ");
        LCD.print(analogRead(IRPIN_RIGHT));
        count = 0;
      }
      count++;
 }
 LCD.print(analogRead(IRPIN_LEFT));
 LCD.print(" ");
 LCD.print(analogRead(IRPIN_RIGHT));
 // wait for IR signal
 while(analogRead(IRPIN_LEFT) > 100 || analogRead(IRPIN_RIGHT) > 100){
 }
 LCD.clear();
  count = 0;


  // drive to tank
  timer = millis();
  while(millis() < timer + 6000){
    state = pos.get();
    pid.run(state);
  }
  while (analogRead(QRD_OUTER_LEFT) < 300 && analogRead(QRD_OUTER_RIGHT)< 300) {
    state = pos.get();
    pid.run(state);
//    if (count == 10) {
//      LCD.clear();
//      LCD.home();
//      LCD.print(analogRead(QRD_RIGHT));
//      LCD.print(" ");
//      LCD.print(analogRead(QRD_LEFT));
//      LCD.print(" ");
//      LCD.print(analogRead(QRD_OUTER_RIGHT));
//      count = 0;
//    }
//    count++;
  }
  LCD.print("turn");
  pid.stop();
  delay(400); //stops before it turns
  BounceSwitchSide.attach(SWITCH_SIDE);
  turn();
  // turn corner
  LCD.clear();
  LCD.home();
  LCD.clear();
  LCD.home();
  LCD.print("Done");
  pid.init(15, 0, 30, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);

  //drive to first hashmark
  // might need to do left and right need to test this
  state=-5; //chage dont 5 dont know why
  circle.lstate=state;

 while(analogRead(QRD_OUTER_LEFT) < THRESH_QRD){
      state = circle.get();
      pid.run(state);
    }
    while(analogRead(QRD_OUTER_LEFT) > THRESH_QRD){
      state = circle.get();
      if(state > 0){state = 0;}
      if(state == -1) {state = -2;}
      pid.run(state);
    }
  while(analogRead(QRD_OUTER_LEFT) < THRESH_QRD){
      state = circle.get();
      pid.run(state);
    }
    while(analogRead(QRD_OUTER_LEFT) > THRESH_QRD){
      state = circle.get();
      if(state > 0){state = 0;}
      if(state == -1) {state = -2;}
      pid.run(state);
    }
  while(analogRead(QRD_OUTER_LEFT) < THRESH_QRD){
      state = circle.get();
      pid.run(state);
    }
  timer = millis();
  while(millis() < timer + 100){
      state = circle.get();
      pid.run(state);
    }

 pid.stop();
 LCD.print("d");
 LCD.clear();
 LCD.home();

 // raise arm
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 BounceLiftUp.update();
 while(!BounceLiftUp.read()){
  BounceLiftUp.update();
 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);

 // turn arm
 
 BounceSwitchArm.attach(SWITCH_ARM_1);
 motor.speed(MOTOR_ARM, 190);
 BounceSwitchArm.update();
 //Arm is switches not encoder !!! Needs to change
 while(!BounceSwitchArm.read()){
    BounceSwitchArm.update();
 }
 motor.speed(MOTOR_ARM, 0);

 digitalWrite(CAR_WASH, HIGH);
 // lower arm

 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 BounceLiftDown.attach(SWITCH_LIFTDOWN);
 BounceLiftDown.update();
 BounceLiftMid.attach(SWITCH_LIFTMID);
 BounceLiftMid.update();
 while(!BounceLiftMid.read() && !BounceLiftDown.read()){
    
    BounceLiftMid.update();
    BounceLiftDown.update();
 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
 delay(400);



 pid.init(15, .01, 30, 1, 150, MOTOR_LEFT, MOTOR_RIGHT);
 circle.lstate = -1; //in case robot came off tape while reversing
 while(hashCount < TOTAL_HASHES){
//  state = circle.get();
  
//  pid.run(state);
 
    BounceSwitchSide.update();
    while(!BounceSwitchSide.rose()){
      state = circle.get();
      if(analogRead(QRD_OUTER_LEFT) > THRESH_QRD){
        if(state > 0){state = 0;}
        if(state == -1) {state = -2;}
      }
      if(state == 5){state = 2;}
      pid.run(state);
      BounceSwitchSide.update();
    }
    count = 1;
    
    while(!BounceSwitchSide.fell()){
      state = circle.get();
      if(analogRead(QRD_OUTER_LEFT) > THRESH_QRD){
        if(state > 0){state = 0;}
        if(state == -1) {state = -2;}
      }
      if(state == 5){ state = 2;}
      pid.run(state);
      count++;
      BounceSwitchSide.update();
    }
    while(analogRead(QRD_OUTER_LEFT) < THRESH_QRD){
      state = circle.get();
      pid.run(state);
    }
    count = 1;
    delay(3);
    while(analogRead(QRD_OUTER_LEFT) > THRESH_QRD){
      state = circle.get();
      if(state > 0){state = 0;}
      if(state == -1) {state = -2;}
      pid.run(state);
      count++;
    }
    if(count >= 2){
     hashCount++;
     LCD.clear();
     LCD.home();
     LCD.print(hashCount);
    }

  }


  while(millis() < timer + 300){
   state = circle.get();
  if(state > 0){state = 0;}
  if(state == -1) {state = -2;}
    pid.run(state);
  }
 pid.stop();
 
 zipline();
 
 exit(0);
}

void turn() {
  motor.speed(MOTOR_LEFT, 200);
  motor.speed(MOTOR_RIGHT, -100); // ONLY FOR 1
  //motor.speed(MOTOR_RIGHT, 50); // ONLY FOR 2
  delay(200);
  LCD.print("flag");
  //while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {} // ONLY FOR 1  
  //while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {} //ONLY FOR 1
  while (analogRead(QRD_CIRCLE_LEFT) < THRESH_QRD) {}  
  while (analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) {}
    
  circle.lstate=-5;
  pid.run(-5);
}
void zipline(){
     // raise arm to zipline
 flag = false;
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 BounceLiftUp.attach(SWITCH_LIFTUP);
 BounceLiftUp.update();
 while(!BounceLiftUp.read()){
  BounceLiftUp.update();
 }

 digitalWrite(MOTOR_LIFT_ON, LOW);

 digitalWrite(CAR_WASH, LOW);
  // turn arm
 
 BounceSwitchArm2.attach(SWITCH_ARM_2);
 BounceSwitchArm2.update();
 motor.speed(MOTOR_ARM, -190);
 //Arm is switches not encoder !!! Needs to change
 while(!BounceSwitchArm2.read()){
    BounceSwitchArm2.update();
 }
 motor.speed(MOTOR_ARM, 0);
 
 LCD.print("a");
 motor.speed(MOTOR_RIGHT,145);
 motor.speed(MOTOR_LEFT, 140);
 BounceSwitchZipline.attach(SWITCH_ZIPLINE);
 BounceSwitchZipline.update();
 while(!BounceSwitchZipline.read()){
   BounceSwitchZipline.update();
 }
 pid.stop();
 LCD.print("b");
 // raise body
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH); 

 BounceLiftDown.attach(SWITCH_LIFTDOWN);
 BounceLiftDown.update();
 timer = millis();
 while(true){
  while(!BounceLiftDown.read()){
    digitalWrite(MOTOR_LIFT_ON, HIGH);
    BounceLiftDown.update();
//    if(millis() > timer + 3000){
//      motor.speed(MOTOR_LEFT, -255);
//      motor.speed(MOTOR_RIGHT, -255);
//    }
  }
  digitalWrite(MOTOR_LIFT_ON, LOW);
  BounceLiftDown.update();
  
 }
}

