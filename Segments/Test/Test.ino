#include  <phys253.h>
#include  <LiquidCrystal.h>
#include  <initialize.h>
void setup() {
  // put your setup code here, to run once:
   #include <phys253setup.txt>
    Serial.begin(9600);
}

void loop() { 
  // put your main code here, to run repeatedly:
  LCD.clear();
  LCD.home();
LCD.print(analogRead(QRD_CIRCLE_RIGHT));
LCD.print(" ");
LCD.print(analogRead(QRD_CIRCLE_LEFT));
LCD.print(" ");
LCD.print(analogRead(QRD_OUTER_RIGHT));
  delay(100);
}


