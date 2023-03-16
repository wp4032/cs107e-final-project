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


void controls_init(void);

int clear_accel(void);
int clear_gyro(void);
void loop(void);

#endif
