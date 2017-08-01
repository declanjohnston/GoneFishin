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

void loop() {
   pid.init(20, .4, 40, 1, 130, MOTOR_LEFT, MOTOR_RIGHT);
  // put your main code here, to run repeatedly:
  circle.lstate = -1; //in case robot came off tape while reversing
 while(hashCount < TOTAL_HASHES){
  state = circle.get();
  
  pid.run(state);
  //if((analogRead(QRD_RIGHT) > THRESH_QRD || analogRead(QRD_LEFT) > THRESH_QRD) && (analogRead(QRD_CIRCLE_RIGHT) > THRESH_QRD || analogRead(QRD_CIRCLE_LEFT) > THRESH_QRD) ){
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
    hashCount++;
    //LCD.clear();
    //LCD.home();
    //LCD.print(hashCount);
    state = -1;
    circle.lstate = -1;
  }
 
 pid.stop();


 
 exit(0);
}
