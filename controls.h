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
  unsigned int min_x;
  unsigned int max_x;
  unsigned int min_y;
  unsigned int max_y;
} control_limits_t;

typedef struct {
signed short accel_x;
signed short accel_y;
signed short accel_z;
} control_accel_t;

typedef struct {
  enum { EMG_ACTIVE, EMG_INACTIVE } action;
  unsigned int x;
  unsigned int y;
  control_limits_t limits;
  unsigned int sensitivity;
  unsigned int threshold;
} control_action_t;

#define SCREEN_X 1960
#define SCREEN_Y 1600

void controls_init(void);

void set_limits(unsigned int screen_x, unsigned int screen_y);
void calibrate(void);

int clear_accel(void);
int clear_gyro(void);

control_accel_t control_read_accel(void);

void loop(void);

#endif
