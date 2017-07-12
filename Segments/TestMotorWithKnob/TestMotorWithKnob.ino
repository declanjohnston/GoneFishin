#include <phys253.h>
#include <LiquidCrystal.h>

int power;
int count;
void setup() {
  // put your setup code here, to run once:
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  power = (knob(6) - 512)/2;
  motor.speed(0, power);
  if(count == 500){
    LCD.clear();
    LCD.home();
    LCD.print(power);
    count = 0;
  }
  count++;
  

}
