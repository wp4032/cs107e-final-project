#include "printf.h"
#include "timer.h"

void print_title(void) {
  printf("Welcome to:\n Dreams Don't Die - Moonwalker\nby: William Pan (williampan@stanford.edu)\nMarch 28, 2023\n");
}

void print_start_calibration(void) {
  printf("\nStarting calibration...\nDo not move...\n");
  timer_delay(1);
  printf("\n[3]");
  timer_delay(1);
  printf("\n[2]");
  timer_delay(1);
  printf("\n[1]\n");
}

void print_calibration_try_again(void) {
  printf("\nRestarting calibration again...\nTry to point your arm straight toward the screen\n");
  timer_delay(1);
  printf("\n[3]");
  timer_delay(1);
  printf("\n[2]");
  timer_delay(1);
  printf("\n[1]\n");
}

void print_calibration_success(void) {
  printf("\n\nCalibration successful!\n");
}

void print_calibration_failure(void) {
  printf("\n\nCalibration failed!\n");
}

void print_error(void) {
  printf("\n\nError occured! Restart the device :(\n");
}