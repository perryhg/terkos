//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

#ifndef _MOTOR_H
#define _MOTOR_H

#define MOT_QUEUE_SIZE                   0x100
#define MOT_DEFAULT_QUEUE_SIZE           5*sizeof(long)
#define MOT_TRAJ_TIMER                   50
#define MOT_HOPPER_SCALE                 11
#define MOT_CAL_SCALE                    11
#define MOT_MAX_PWM                      0xff
#define MOT_MCURRENT_SCALE               4
#define MOT_MCURRENT_MEASUREMENTS        16
#define MOT_BEMF_MEASUREMENTS            16
#define MOT_COMMAND_SCALE                8
#define MOT_POSITION_SCALE               8

// mode bitmap
#define MOT_MODE_BEMF                    0
#define MOT_MODE_QUADRATURE              1

void mot_periodic(void);
void mot_init(void *addr);
void mot_exit(void);
void mot_axis_init(int axis);
void mot_axis_exit(int axis);
int mot_axis_write(int axis, char *buf, unsigned int count);
int mot_axis_read(int axis, char *buf, unsigned int count);
void mot_axis_reset(int axis);

int mot_set_period(unsigned int axis, unsigned int period);
int mot_get_frequency(unsigned int axis, unsigned int *frequency);
int mot_get_status(unsigned int axis, unsigned short *status);
int mot_set_mode(unsigned int axis, unsigned long mode);
int mot_get_position(unsigned int axis, Axis_position *position);       
int mot_get_position_vector(unsigned int axis, Axis_position position[]);  
int mot_set_pwm(unsigned int axis, int pwm);
int mot_get_pwm(unsigned int axis, int *pwm);

// other functions..
int mot_set_bemf_offset(unsigned int axis, int offset);
int mot_set_position(unsigned int axis, Axis_position pos);
int mot_get_mcurrent(unsigned int axis, int *mcurrent);
int mot_get_mcurrent_vector(unsigned int, int mcurrent[]);
int mot_get_velocity(unsigned int axis, long *velocity);
int mot_get_velocity_vector(unsigned int axis, long velocity[]);

int mot_set_bemf_calibrate(unsigned int axis, unsigned int cal);
int mot_set_bemf_timing(unsigned int axis, unsigned int wait_interval);

// closed loop functions
int mot_set_pidv_gains(unsigned int axis, unsigned int p_gain, 
		      unsigned int i_gain, unsigned int d_gain, 
		      unsigned int v_gain);
int mot_set_max_error(unsigned int axis, unsigned int error);
int mot_set_max_integral(unsigned int axis, unsigned int integral);
int mot_get_done(unsigned int axis, unsigned short *done);
int mot_set_hold(unsigned int axis, unsigned int hold);
int mot_stop(unsigned int axis);
int mot_get_bemf(unsigned int axis, long *bemf);
int mot_flush_write_queue(unsigned int axis);
int mot_set_write_queue_size(unsigned int axis, unsigned int size);
int mot_set_read_queue_size(unsigned int axis, unsigned int size);

#endif
