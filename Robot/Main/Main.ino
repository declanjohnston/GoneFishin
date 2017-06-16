#include <phys253.h>
#include <LiquidCrystal.h>
#include <DeclanClass.h>

////////////////////////////////////////////////
// TINAH Demo Program - UBC Engineering Physics 253
// (nakane, 2016 May 5) - minor updates for new ServoTINAH setup.   
// (nakane, 2015 May 7) -  Update for IDE 1.6.4
// (nakane, 2015 Jan 2)  - Updated for use in Arduino IDE (1.0.6)
/////////////////////////////////////////////////

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  DeclanClass hi;
  Serial.print(hi.test());
}




void loop()
{
}

