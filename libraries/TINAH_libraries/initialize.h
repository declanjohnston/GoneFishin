





#define MOTOR_LEFT 2
#define MOTOR_RIGHT 1
#define MOTOR_LIFT_ON 4// high is on, low is off
#define MOTOR_LIFT_DIRECTION 3 // high is up, low is down
#define MOTOR_ARM 3 
#define THRESH_IR 100   // was at 400
#define THRESH_TIMER 1400 // need to determine experimentally
#define THRESH_QRD 100
#define IRPIN_LEFT 7
#define IRPIN_RIGHT 0
#define QRD_LEFT 3
#define QRD_RIGHT 2
#define QRD_CIRCLE_LEFT 5
#define QRD_CIRCLE_RIGHT 4
#define QRD_OUTER_RIGHT 1
#define QRD_OUTER_LEFT 6
#define ENCODER_RAISEARM 1000
#define ENCODER_LOWERARM 25 // 26 for static encouragers
#define TOTAL_HASHES 12 // might need to change maybe 8
#define SWITCH_ZIPLINE 11
#define SWITCH_LIFTUP 14
#define SWITCH_LIFTDOWN 9
#define SWITCH_ARM_1 12
#define SWITCH_ARM_2 10
#define SWITCH_TRACK 15
#define SWITCH_LIFTMID 8
#define CAR_WASH 6
#define SWITCH_SIDE 15


void initPins(){
portMode(0,OUTPUT);

}