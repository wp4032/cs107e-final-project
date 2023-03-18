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
  enum { EMG_ACTIVE, EMG_INACTIVE } action;
  signed int x;
  signed int y;
  control_limits_t limits;
  unsigned int sensitivity;
} control_action_t;


void controls_init(void);

int clear_accel(void);
int clear_gyro(void);
void loop(void);

#endif
