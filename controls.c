#include "accel.h"
#include "interrupts.h"
#include "messages.h"
#include "printf.h"
#include "timer.h"
#include "peripherals.h"
#include "controls.h"

static signed short accel_x_offset = 0;
static signed short accel_y_offset = 0;
static signed short accel_z_offset = 0;
static signed short gyro_x_offset = 0;
static signed short gyro_y_offset = 0;
static signed short gyro_z_offset = 0;

static control_limits_t limits;
static control_action_t control;

int clear_accel(void);
int clear_gyro(void);

void controls_init(void) {
  set_limits(SCREEN_X, SCREEN_Y);
  set_location(SCREEN_X, SCREEN_Y);
  set_sensitivity_threshold(SENSITIVITY, THRESHOLD);
  clear_accel();
  clear_gyro();
  // calibrate();
}


static unsigned int abs(signed int a) {
  if (a < 0) {
    return a * -1;
  }
  return a;
}


void set_limits(signed int screen_x, signed int screen_y) {
  limits.min_x = 0;
  limits.max_x = screen_x;
  limits.min_y = 0;
  limits.max_y = screen_y;
  control.limits = limits;
}


void set_location(signed int screen_x, signed int screen_y) {
  control.x = screen_x / 2;
  control.y = screen_y / 2;
  control.action = EMG_INACTIVE;
}


void set_sensitivity_threshold(signed int sensitivity, signed int threshold) {
  control.sensitivity = sensitivity;
  control.threshold = threshold;
}


void calibrate(void) {
  int attempts = 0;
  while (clear_accel() || clear_gyro()) {
    if (attempts == 2) {
      print_calibration_failure();
      error_led();
      return;
    }
    print_calibration_try_again();
    attempts++;
  }
  print_calibration_success();
}


int clear_accel(void) {
  accel_xyz(&accel_x_offset, &accel_y_offset, &accel_z_offset);
  // If orientation is not flat in the y direction or the z direction --> will return false
  // X orientation has some more tolerance 
  return (abs(accel_x_offset/16) > 250 || abs(accel_y_offset/16) > 50 || abs(accel_z_offset/16) > 1100 || abs(accel_z_offset/16) < 900);
}


int clear_gyro(void) {
  gyro_xyz(&gyro_x_offset, &gyro_y_offset, &gyro_z_offset);
  // If angular rotation in x, y, z direction is not still --> will return false
  return (abs(gyro_x_offset/16) > 50 || abs(gyro_y_offset/16) > 50 || abs(gyro_z_offset/16) > 50);
}


control_accel_t control_read_accel(void) {
  control_accel_t result = {
    (accel_get_x() - accel_x_offset)/16,
    (accel_get_y() - accel_y_offset)/16, 
    (accel_get_z() - accel_z_offset)/16
  };
  printf("accel=(%d,%d,%d)\n", result.accel_x, result.accel_y, result.accel_z);
  return result;
}


control_gyro_t control_read_gyro(void) {
  control_gyro_t result = {
    (gyro_get_x() - gyro_x_offset)/16, 
    (gyro_get_y() - gyro_y_offset)/16, 
    (gyro_get_z() - gyro_z_offset)/16,
  };
  // printf("gyro=(%d,%d,%d)\n", result.gyro_x, result.gyro_y, result.gyro_z);
  return result;
}


void control_read_action(control_action_t *ctrl) {
  control_accel_t accel = control_read_accel();
  control_gyro_t gyro = control_read_gyro();

  signed int delta_y = abs(accel.accel_y) > ctrl->threshold ? accel.accel_y / ctrl->sensitivity : 0;
  signed int delta_z = abs(accel.accel_z) > ctrl->threshold ? accel.accel_z / ctrl->sensitivity : 0;

  printf("dx: %d, dy: %d\n", delta_y, delta_z);

  ctrl->x += (delta_y + ctrl->x < ctrl->limits.max_x && delta_y + ctrl->x > ctrl->limits.min_x) ? delta_y : 0; 
  ctrl->y += (delta_z + ctrl->y < ctrl->limits.max_y && delta_z + ctrl->y > ctrl->limits.min_y) ? delta_z : 0; 

  printf("x: %d, y: %d\n", ctrl->x, ctrl->y);
}

void loop(void) {
  while(1) { 
    success_led_on();
    control_read_action(&control);
    timer_delay_ms(10);
    success_led_off();
    timer_delay_ms(10);
	}
}