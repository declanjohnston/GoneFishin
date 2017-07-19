#include <phys253.h>
#include <LiquidCrystal.h>

void setup() {
  // put your setup code here, to run once:
#include <phys253setup.txt>
  Serial.begin(9600) ;
}

void loop() {
  // put your main code here, to run repeated:
  LCD.clear();
  LCD.home();
  LCD.print(knob(6)/4);
  motor.speed(0,knob(6));
  while(digitalRead(0) == LOW);

  motor.speed(0,0);
  exit(0);
}
