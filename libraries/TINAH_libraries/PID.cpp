#include <Arduino.h>
#include "PID.h"


PID::PID(motorClass motorIn){
	motor=motorIn;
}

void PID::init(int derivative, int integral, int proportional, int gain, int setSpeed, int motorLeft, int motorRight){
	D=derivative;
	I=integral;
	P=proportional;
	G=gain;
	q = 1;
    	m = 1;
	lstate = 0;
    	recerr = 0;
	count = 0;
	Speed = setSpeed;
	leftMotor = motorLeft;
	rightMotor = motorRight;
	i=0;
		
}

void PID::run(int xpos){
	state = xpos;

	if (!(state==lstate))
 	{
     		recerr=lstate;
     		q=m;
    		m=1;
  	}
  	// Calculations of correction
  	p=P*state;
  	d=(int)((float)D*(float)(state-recerr)/(float)(q+m));
	i= i+state*I;
  	cor = G*(p+d+i);
  	// Writing to motors
  	motor.speed(leftMotor,Speed+cor);
  	motor.speed(rightMotor,Speed-cor);
  
  	m=m+1;
  	count = count + 1;
  	lstate=state;
}

void PID::stop(){
	motor.speed(leftMotor,0);
  	motor.speed(rightMotor,0);
}