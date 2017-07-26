#include  <phys253.h>
#include  <LiquidCrystal.h>
    int leftSen, rightSen;
    int state;
    int D = 10;
    int G = 1;
    int P;
    int threshold = 100;
    byte LEFTPIN = 3;
    byte RIGHTPIN = 1;
    bool left;
    bool right;
    byte count = 100;
    int Speed = 255;
    int lstate = 0;
    int recerr = 0;
    int q = 1;
    int m =1;
    int p;
    int d;
    int cor;
    byte MOTOR_RIGHT = 1;
    byte MOTOR_LEFT = 2;
    
void setup()
{
    #include <phys253setup.txt>
    Serial.begin(9600);  
   
    pinMode(0, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    
    
    
}

void loop() {
  
  leftSen = analogRead(LEFTPIN);
  rightSen = analogRead(RIGHTPIN);
  //Printing to screen and reading knob values
  if(count == 100){
    Speed = knob(7)/4;
    P = knob(6)/5;
    LCD.clear();
    LCD.home();
    LCD.print("l=");
    LCD.print(leftSen);
    LCD.print(" r=");
    LCD.print(rightSen);
    LCD.setCursor(0, 1);
    LCD.print(" P=");
    LCD.print(P);
    LCD.print(" S=");
    LCD.print(Speed);
    LCD.print(" X=");
    LCD.print(state);
    count = 0;
  }
  // Reading of the sensors
  if(leftSen> threshold){
    left = true;
  }
  else{
    left = false;
  }
  if(rightSen > threshold){
    right = true;
  }
  else{
    right =  false;
  }
  // Determining the state
  if(left && right) state = -1;
  if(left && !right) state = 0;
  if(!left && right) state = -3;
  if(!left && !right){
    if(lstate<0){ // lstateis the most recent state of the system, allows the robot to decide which side it is on
      state = -5;
    }
    else{
      state = 5;
    }
  }
  // Resetting the derivative calculation when state changes
  if (!(state==lstate))
  {
     recerr=lstate;
     q=m;
     m=1;
  }
  // Calculations of correction
  p=P*state;
  d=(int)((float)D*(float)(state-recerr)/(float)(q+m));
  cor = G*(p+d);
  // Writing to motors
  motor.speed(MOTOR_RIGHT,Speed+cor);
  motor.speed(MOTOR_LEFT,Speed-cor);
  
  m=m+1;
  count = count + 1;
  lstate=state;
}
