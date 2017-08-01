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
//State pos=State(QRD_LEFT, QRD_RIGHT,THRESH_QRD, QRD_CIRCLE_LEFT);
State circle=State(QRD_CIRCLE_LEFT, QRD_CIRCLE_RIGHT, THRESH_QRD, QRD_RIGHT);

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  initPins();
  // remember to initialize values
  // derivative, integral, proportional,gain, speed
  pid.init(10,0,40,1,160, MOTOR_LEFT, MOTOR_RIGHT);
  
}

void loop(){
  // drive around circle
 flag = false;
 while(hashCount < TOTAL_HASHES){
      state = circle.straddle();
      pid.run(state);
      /*if(flag){
        if(!circle.out){ // again might want ti make it right and left not jut one
          flag = false;
          //hashCount++;
          LCD.clear();
          LCD.home();
          LCD.print(hashCount);
        }
      }
      else if(circle.out){
        flag = true;
      }*/
 }
 pid.stop();
 exit(0);
}

