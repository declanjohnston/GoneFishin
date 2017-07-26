#ifndef State_h	
#define State_h

// include types & constants of Wiring core API
//#include "WConstants.h"


class State
{
	public:
		//constructor
		State(byte qrdLeft, byte qrdRight, byte threshold, byte qrdRightOut);
		
		int get();
		
		int straddle();
		
		int tank();
		int lstate;

	private:
		int state;
		byte QRD_LEFT;
		byte QRD_RIGHT;
		bool left;
		bool right;
		byte THRESH_QRD;
		byte QRD_OUT;
	public:
		bool out;
};
#endif