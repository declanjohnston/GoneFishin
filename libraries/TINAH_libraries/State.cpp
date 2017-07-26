#include <Arduino.h>
#include "State.h"


State::State(byte qrdLeft, byte qrdRight, byte threshold, byte qrdOut){
	QRD_LEFT = qrdLeft;
	QRD_RIGHT = qrdRight;
	QRD_OUT = qrdOut;
	THRESH_QRD = threshold;
	lstate = 0;
}


		


int State::get(){
	if(analogRead(QRD_LEFT)> THRESH_QRD){
        left = true;
      }
      else{
        left = false;
      }
      if(analogRead(QRD_RIGHT) > THRESH_QRD){
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
        if(lstate<=0){ // lstateis the most recent state of the system, allows the robot to decide which side it is on
          state = -5;
        }
        else{
          state = 5;
        }
      }
	lstate = state;
	return state;
}

int State::straddle(){
	  if(analogRead(QRD_LEFT)> THRESH_QRD){
        left = true;
      }
      else{
        left = false;
      }
	  if(analogRead(QRD_RIGHT) > THRESH_QRD){
        right = true;
      }
      else{
        right =  false;
      }
	  if(analogRead(QRD_OUT) > THRESH_QRD){
       out = true;
      }
      else{
        out =  false;
      }
      // Determining the state
      if(left && right) state = -1;
      if(left && !right) state = 0;
      if(!left && right) state = -3;
      if(!left && !right){
        if(lstate<0){ // lstate is the most recent state of the system, allows the robot to decide which side it is on
          state = -5;
        }
        else{
          state = 5;
        }
      }
	  if(left && right && out){
		  state = 1;
	  }
	lstate = state;
	return state;
}

int State::tank(){
	  if(analogRead(QRD_LEFT)> THRESH_QRD){
        left = true;
      }
      else{
        left = false;
      }
	  if(analogRead(QRD_RIGHT) > THRESH_QRD){
        right = true;
      }
      else{
        right =  false;
      }
	  if(analogRead(QRD_OUT) > THRESH_QRD){
       out = true;
      }
      else{
        out =  false;
      }
      // Determining the state
      if(left && right) state = 0;
      if(left && !right) state = 1;
      if(!left && right) state = -1;
      if(!left && !right){
        if(lstate<=0){ // lstate is the most recent state of the system, allows the robot to decide which side it is on
          state = -5;
        }
        else{
          state = 5;
        }
      }
	  if(left && right && out){
		  state = 1;
	  }
	lstate = state;
	return state;
}