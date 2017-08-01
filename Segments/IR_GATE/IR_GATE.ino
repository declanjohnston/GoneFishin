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
 LCD.print(" ");
 LCD.print("Done");
 exit(0);
}
