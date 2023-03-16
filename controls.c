#include "accel.h"
#include "interrupts.h"
#include "messages.h"
#include "printf.h"
#include "timer.h"

static signed short accel_x_offset = 0;
static signed short accel_y_offset = 0;
static signed short accel_z_offset = 0;
static signed short gyro_x_offset = 0;
static signed short gyro_y_offset = 0;
static signed short gyro_z_offset = 0;

int clear_accel(void);
int clear_gyro(void);

void controls_init(void) {
  int attempts = 0;
  while (!clear_accel() || !clear_gyro()) {
    if (attempts == 2) {
      print_calibration_failure();
      return;
    }
    attempts++;
  }
  print_calibration_success();
}

static unsigned int abs(signed int a) {
  if (a < 0) {
    return a * -1;
  }
  return a;
}

int clear_accel(void) {
  accel_xyz(&accel_x_offset, &accel_y_offset, &accel_z_offset);
  // If orientation is not flat in the y direction or the z direction --> will return false
  // X orientation has some more tolerance 
  return (abs(accel_x_offset) > 150 || abs(accel_y_offset) > 50 || abs(accel_z_offset) > 1100 || abs(accel_z_offset) < 900);
}

int clear_gyro(void) {
  gyro_xyz(&gyro_x_offset, &gyro_y_offset, &gyro_z_offset);
  // If angular rotation in x, y, z direction is not still --> will return false
  return (abs(gyro_x_offset) > 50 || abs(gyro_y_offset) > 50 || abs(gyro_z_offset) > 50);
}

void loop(void) {
  while(1) { 
    printf("accel=(%dmg,%dmg,%dmg)\n", (accel_get_x() - accel_x_offset)/16, (accel_get_y() - accel_y_offset)/16, (accel_get_z() - accel_z_offset)/16);
    printf("gyro=(%d, %d, %d)\n", (gyro_get_x() - gyro_x_offset)/16, (gyro_get_y() - gyro_y_offset)/16, (gyro_get_z() - gyro_z_offset)/16);
    timer_delay_ms(200);
	}
}