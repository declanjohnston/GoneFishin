#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>

PID pid=PID(motor);

byte MOTORLEFT = 0;
byte MOTORRIGHT = 1;

int THRESH_IR = 10;
int THRESH_TIMER;
int THRESH_QRD;
byte IRPIN = 1;
byte LEFT_OUT_QRD;
byte RIGHT_OUT_QRD;
byte QRD_LEFT;
byte QRD_RIGHT;

int state;
int lstate;
bool left;
bool right;
int timer;
State pos=State(QRD_LEFT, QRD_RIGHT,THRESH_QRD);
void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;

  pid.init(0,0,0,0,0, MOTORLEFT, MOTORRIGHT);
}

void loop()
{
 timer = millis();
 while(timer , THRESH_TIMER){
    
      state = pos.get();
      pid.run(state);
      
 }
 while(analogRead(IRPIN) < THRESH_IR){}
 while(analogRead(LEFT_OUT_QRD) < THRESH_QRD && analogRead(RIGHT_OUT_QRD) < THRESH_QRD){
      state = pos.get();
      pid.run(state);
 }
  
}

