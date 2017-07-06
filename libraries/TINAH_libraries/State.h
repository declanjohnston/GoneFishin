#ifndef State_h	
#define State_h

// include types & constants of Wiring core API
//#include "WConstants.h"


class State
{
	public:
		//constructor
		State(byte qrdLeft, byte qrdRight, byte threshold);
		
		int get();

	private:
		int state;
		int lstate;
		byte QRD_LEFT;
		byte QRD_RIGHT;
		bool left;
		bool right;
		byte THRESH_QRD;
		
};
#endif