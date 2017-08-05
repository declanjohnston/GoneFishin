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
  pid.init(20, 0.30, 60, 1, 255, MOTOR_LEFT, MOTOR_RIGHT);
  BounceLiftMid.interval(3);
  BounceLiftUp.interval(3);
  BounceLiftDown.interval(3);
  BounceLiftUp.attach(SWITCH_LIFTUP);
  BounceLiftDown.attach(SWITCH_LIFTDOWN);
  BounceSwitchZipline.interval(3);
  BounceSwitchArm.interval(3);
  BounceSwitchArm2.interval(3);
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

 pid.init(20, 0.30, 60, 1, 255, MOTOR_LEFT, MOTOR_RIGHT);
  // drive to tank
  while (analogRead(QRD_OUTER_LEFT) < 300 && analogRead(QRD_OUTER_RIGHT)< 300) {
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
  LCD.print("turn");
  pid.stop();
  delay(400); //stops before it turns
  turn();
  // turn corner
  LCD.clear();
  LCD.home();
  LCD.clear();
  LCD.home();
  LCD.print("Done");
  pid.init(15, .4, 30, 1, 150, MOTOR_LEFT, MOTOR_RIGHT);

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



 pid.init(15, .4, 30, 1, 150, MOTOR_LEFT, MOTOR_RIGHT);
 circle.lstate = -1; //in case robot came off tape while reversing
 while(hashCount < TOTAL_HASHES){
  state = circle.get();
  
  pid.run(state);
  //if((analogRead(QRD_RIGHT) > THRESH_QRD || analogRead(QRD_LEFT) > THRESH_QRD) && (analogRead(QRD_CIRCLE_RIGHT) > THRESH_QRD || analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) ){
    
    while(analogRead(QRD_OUTER_LEFT) < THRESH_QRD){
      state = circle.get();
      pid.run(state);
    }
    count = 1;
    while(analogRead(QRD_OUTER_LEFT) > THRESH_QRD){
      state = circle.get();
      if(state > 0){state = 0;}
      if(state == -1) {state = -2;}
      pid.run(state);
      count++;
    }
    if(count >= 3){
     hashCount++;
     LCD.clear();
     LCD.home();
     LCD.print(hashCount);
    }
    state = -1;
    circle.lstate = -1;
  }
//  while(millis() < timer + 300){
//   state = circle.get();
//  if(state > 0){state = 0;}
//  if(state == -1) {state = -2;}
//    pid.run(state);
//  }
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
 pid.init(0,1,35,1,255, MOTOR_LEFT, MOTOR_RIGHT);
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
 motor.speed(MOTOR_RIGHT,140);
 motor.speed(MOTOR_LEFT, 120);
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
 while(true){
  while(!BounceLiftDown.read()){
    digitalWrite(MOTOR_LIFT_ON, HIGH);
    BounceLiftDown.update();
  }
  digitalWrite(MOTOR_LIFT_ON, LOW);
  BounceLiftDown.update();
  
 }
}

