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
Bounce BounceLiftUp = Bounce();
Bounce BounceLiftDown = Bounce();
Bounce BounceSwitchZipline = Bounce();
Bounce BounceEncoder = Bounce();

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
  pid.init(15, .4, 30, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);
  BounceLiftUp.interval(3);
  BounceLiftUp.attach(SWITCH_LIFTUP);
  BounceLiftDown.interval(3);
  BounceLiftDown.attach(SWITCH_LIFTDOWN);
  BounceSwitchZipline.interval(3);
  BounceSwitchZipline.attach(SWITCH_ZIPLINE);
  BounceEncoder.interval(3);
  BounceEncoder.attach(ENCODER_LIFTPIN);
}

void loop(){
  flag = false;
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 BounceLiftUp.update();
 while(!BounceLiftUp.read()){
  BounceLiftUp.update();
 }

 digitalWrite(MOTOR_LIFT_ON, LOW);

 encoderCount = 0;
 digitalWrite(MOTOR_LIFT_DIRECTION, LOW);
 digitalWrite(MOTOR_LIFT_ON, HIGH);
 BounceLiftDown.attach(SWITCH_LIFTDOWN);
 BounceLiftDown.update();
 while(encoderCount < ENCODER_LOWERARM && !BounceLiftDown.read()){
    BounceEncoder.update();
    if(BounceEncoder.rose()){
      encoderCount++;
      LCD.clear();
      LCD.print(encoderCount);
    }
    BounceLiftDown.update();
 }
 digitalWrite(MOTOR_LIFT_ON, LOW);
  
  // drive around circle
  circle.lstate = -1; //in case robot came off tape while reversing
 digitalWrite(CAR_WASH, HIGH);
 while(hashCount < TOTAL_HASHES){
  state = circle.get();
  
  pid.run(state);
  //if((analogRead(QRD_RIGHT) > THRESH_QRD || analogRead(QRD_LEFT) > THRESH_QRD) && (analogRead(QRD_CIRCLE_RIGHT) > THRESH_QRD || analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) ){
    
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
    hashCount++;
    LCD.clear();
    LCD.home();
    LCD.print(hashCount);
    state = -1;
    circle.lstate = -1;
  }
 
 pid.stop();
 digitalWrite(CAR_WASH, LOW);
 exit(0);
}

