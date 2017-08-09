#include <avr/interrupt.h>
#include <phys253.h>
#include <LiquidCrystal.h>
#include <initialize.h>




void setup()
{
  #include <phys253setup.txt>

}
 
void loop()
{
  motor.speed(MOTOR_LEFT, 255);
  motor.speed(MOTOR_RIGHT,255);
  delay(500);
    motor.speed(MOTOR_LEFT, -255);
  motor.speed(MOTOR_RIGHT,-255);
  delay(500);
}



 

