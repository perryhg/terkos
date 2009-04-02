#ifndef _QEMOTOR_H
#define _QEMOTOR_H

#ifndef TRUE
#define TRUE                   1
#endif
#ifndef FALSE               
#define FALSE                  0
#endif

#define QEMOT_MAJOR_NUMBER     214
#define QEMOT_MINOR_NUMBER     0

#define QEMOT_NUM_MOTORS       4
#define QEMOT_BASE_ADDR        0x20000000
#define QEMOT_WIDTH            0x400
#define QEMOT_VECTOR           0

typedef long Axis_position;
typedef int (*qe_motor_func)(unsigned int, long, long, long, long, long);
#define QEMOT_NUM_ARGS         5

#define MOT_SET_PERIOD           0
#define MOT_GET_FREQUENCY        1
#define MOT_GET_STATUS           2
#define MOT_SET_MODE             3
#define MOT_GET_POSITION         4
#define MOT_GET_POSITION_VECTOR  5
#define MOT_SET_PWM              6
#define MOT_GET_PWM              7
#define MOT_SET_BEMF_OFFSET      8
#define MOT_SET_POSITION         9
#define MOT_GET_MCURRENT         10
#define MOT_GET_MCURRENT_VECTOR  11
#define MOT_GET_VELOCITY         12
#define MOT_GET_VELOCITY_VECTOR  13
#define MOT_SET_BEMF_CALIBRATE   14
#define MOT_SET_BEMF_TIMING      15
#define MOT_SET_PIDV_GAINS       16
#define MOT_SET_MAX_ERROR        17
#define MOT_SET_MAX_INTEGRAL     18
#define MOT_GET_DONE             19
#define MOT_SET_HOLD             20
#define MOT_GET_BEMF             21
#define MOT_FLUSH_WRITE_QUEUE    22
#define MOT_SET_WRITE_QUEUE_SIZE 23
#define MOT_STOP                 24
#define MOT_SET_READ_QUEUE_SIZE  25

#endif
