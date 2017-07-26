





#define MOTOR_LEFT 2
#define MOTOR_RIGHT 1
#define MOTOR_LIFT_ON 7// high is on, low is off
#define MOTOR_LIFT_DIRECTION 6 // high is up, low is down
#define MOTOR_ARM 3 
#define THRESH_IR 300   // was at 400
#define THRESH_TIMER 1000 // need to determine experimentally
#define THRESH_QRD 100
#define IRPIN_LEFT 3
#define IRPIN_RIGHT 0
#define QRD_LEFT 2
#define QRD_RIGHT 1
#define QRD_CIRCLE_LEFT 6
#define QRD_CIRCLE_RIGHT 4
#define QRD_OUTER_RIGHT 7
#define ENCODER_RAISEARM 100
#define ENCODER_LOWERARM 20
#define ENCODER_LIFTPIN 8
#define TOTAL_HASHES 7 // might need to change maybe 8
#define SWITCH_ZIPLINE 11
#define SWITCH_LIFTUP 14
#define SWITCH_LIFTDOWN 9
#define SWITCH_ARM_1 12
#define SWITCH_ARM_2 10



void initPins(){
portMode(0,OUTPUT);

}