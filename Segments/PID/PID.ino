#include  <phys253.h>
#include  <LiquidCrystal.h>
    int leftSen, rightSen;
    int state;
    int D;
    int G;
    int P;
    int threshold = 50;
    byte LEFTPIN = 0;
    byte RIGHTPIN = 5;
    bool left;
    bool right;
    byte count = 100;
    int Speed = 350;
    int lstate = 0;
    int recerr = 0;
    int q = 1;
    int m =1;
    int p;
    int d;
    int cor;
    
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
    D= knob(7)/5;
    P= knob(6)/5;
    LCD.clear();
    LCD.home();
    LCD.print("l=");
    LCD.print(leftSen);
    LCD.print(" r=");
    LCD.print(rightSen);
    LCD.setCursor(0, 1);
    LCD.print(" P=");
    LCD.print(P);
    LCD.print(" D=");
    LCD.print(D);
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
  if(left && right) state = 0;
  if(left && !right) state = -1;
  if(!left && right) state = 1;
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
  motor.speed(1,Speed+cor);
  motor.speed(0,Speed-cor);
  
  m=m+1;
  count = count + 1;
  lstate=state;
}
