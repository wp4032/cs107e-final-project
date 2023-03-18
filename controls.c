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

int clear_accel(void);
int clear_gyro(void);

void controls_init(void) {
  set_limits(SCREEN_X, SCREEN_Y);
  calibrate();
}

static unsigned int abs(signed int a) {
  if (a < 0) {
    return a * -1;
  }
  return a;
}

void set_limits(unsigned int screen_x, unsigned int screen_y) {
  limits.min_x = 0;
  limits.max_x = screen_x;
  limits.min_y = 0;
  limits.max_y = screen_y;
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
  printf("accel=(%dmg,%dmg,%dmg)\n", result.accel_x, result.accel_y, result.accel_z);
  return result;
}

void loop(void) {
  while(1) { 
    success_led_on();
    control_read_accel();
    printf("gyro=(%d, %d, %d)\n", (gyro_get_x() - gyro_x_offset)/16, (gyro_get_y() - gyro_y_offset)/16, (gyro_get_z() - gyro_z_offset)/16);
    timer_delay_ms(100);
    success_led_off();
    timer_delay_ms(100);
	}
}