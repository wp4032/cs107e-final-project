#ifndef MESSSAGES_H
#define MESSSAGES_H

/*
 * Print and draw messages for the interface 
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 28, 2023
 */


void print_title(void);


// DRAW FUNCTIONS
void draw_start_calibration(void);
void draw_calibration_message(int x, int y, int r, int i);
void draw_calibration_success(void);
void draw_calibration_failure(void);
void draw_calibration_try_again(void);
void draw_error(void);


// PRINT FUNCTIONS
void print_start_calibration(void);
void print_calibration_message(int x, int y, int r, int i);
void print_calibration_success(void);
void print_calibration_failure(void);
void print_calibration_try_again(void);
void print_error(void);

#endif
