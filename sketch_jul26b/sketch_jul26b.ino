#include <phys253.h>
#include <LiquidCrystal.h>
#include <PID.h>
#include <State.h>
#include <initialize.h>
#include <avr/interrupt.h>




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
State pos=State(QRD_LEFT, QRD_RIGHT,THRESH_QRD, QRD_CIRCLE_LEFT);
State circle=State(QRD_CIRCLE_LEFT, QRD_CIRCLE_RIGHT, THRESH_QRD, QRD_RIGHT);
///////
volatile unsigned int INT_0 = 0;
 
ISR(INT0_vect) {LCD.clear(); LCD.home(); LCD.print("INT0: "); LCD.print(INT_0++);};
 

//////////////////
void setup()
{
#include <phys253setup.txt>
enableExternalInterrupt(INT0, RISING);
  Serial.begin(9600) ;
  initPins();
  // remember to initialize values
  // derivative, integral, proportional,gain, speed
  pid.init(20,0,35,1,255, MOTOR_LEFT, MOTOR_RIGHT);
  
}
void loop(){
  //raise arm
 LCD.clear();
 LCD.home();
 digitalWrite(MOTOR_LIFT_DIRECTION, HIGH);
 if(digitalRead(SWITCH_LIFTUP) != LOW){
   
   digitalWrite(MOTOR_LIFT_ON, HIGH);
 }
 else if(digitalRead(SWITCH_LIFTUP) == LOW)
 {
  digitalWrite(MOTOR_LIFT_ON, LOW);
 }

}

ISR(INT0_vect) {LCD.clear(); LCD.home(); LCD.print("INT0: "); LCD.print(INT_0++);};

/*  Enables an external interrupt pin
INTX: Which interrupt should be configured?
    INT0    - will trigger ISR(INT0_vect)
    INT1    - will trigger ISR(INT1_vect)
    INT2    - will trigger ISR(INT2_vect)
    INT3    - will trigger ISR(INT3_vect)
mode: Which pin state should trigger the interrupt?
    LOW     - trigger whenever pin state is LOW
    FALLING - trigger when pin state changes from HIGH to LOW
    RISING  - trigger when pin state changes from LOW  to HIGH 
*/
void enableExternalInterrupt(unsigned int INTX, unsigned int mode)
{
  if (INTX > 3 || mode > 3 || mode == 1) return;
  cli();
  /* Allow pin to trigger interrupts        */
  EIMSK |= (1 << INTX);
  /* Clear the interrupt configuration bits */
  EICRA &= ~(1 << (INTX*2+0));
  EICRA &= ~(1 << (INTX*2+1));
  /* Set new interrupt configuration bits   */
  EICRA |= mode << (INTX*2);
  sei();
}
 
void disableExternalInterrupt(unsigned int INTX)
{
  if (INTX > 3) return;
  EIMSK &= ~(1 << INTX);
}
