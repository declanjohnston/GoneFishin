/*
 * TINAH Template Program - UBC Engineering Physics 253
 * (nakane, 2015 Jan 2)  - Updated for use in Arduino IDE (1.0.6)
 */
 
#include  <phys253.h>
#include  <LiquidCrystal.h>
float power;

void setup()
{
    #include <phys253setup.txt>
    Serial.begin(9600);  
    
}
 
void loop()
{
  power = (knob(6)-512)/2;  // knob gives values from 0-1023
  LCD.clear();
  LCD.home();
  LCD.print("hi");
  LCD.print(power);
  motor.speed(0,power); 
//  RCServo0.write(knob(6)/10); // servo takes 0-180
  
  delay(200);
//  power = 1000;
//  motor.speed(0,power);
//  delay(2000);
  
}
