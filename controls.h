#ifndef CONTROLS_H
#define CONTROLS_H

/*
 * Outputs x, y, z controls for the
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 * to have spatial awareness of the device
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 28, 2023
 */

typedef struct {
  signed int min_x;
  signed int max_x;
  signed int min_y;
  signed int max_y;
} control_limits_t;

typedef struct {
  signed short accel_x;
  signed short accel_y;
  signed short accel_z;
} control_accel_t;

typedef struct {
  signed short gyro_x;
  signed short gyro_y;
  signed short gyro_z;
} control_gyro_t;

typedef struct {
  enum { EMG_INACTIVE, EMG_ACTIVE } action;
  signed int x;
  signed int y;
  control_limits_t limits;
  signed int sensitivity;
  signed int threshold;
} control_action_t;

#define SCREEN_X 1960
#define SCREEN_Y 1600
#define THRESHOLD 50
#define SENSITIVITY 30 

void controls_init(void);

void set_limits(signed int screen_x, signed int screen_y);
void set_location(signed int screen_x, signed int screen_y);
void set_sensitivity_threshold(signed int sensitivity, signed int threshold);
void calibrate(void);

int clear_accel(void);
int clear_gyro(void);

control_accel_t control_read_accel(void);
control_gyro_t control_read_gyro(void);
void control_read_action(control_action_t *ctrl);

void loop(void);

#endif
