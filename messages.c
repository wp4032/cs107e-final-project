#include "printf.h"
#include "gl.h"
#include "settings.h"
#include "timer.h"
#include "messages.h"
#include "accel.h" // delete after

static const char *strings[5] = {
  "center",
  "top left",
  "top right",
  "bottom left",
  "bottom right",
};

void print_title(void) {
  printf("Welcome to:\n Dreams Don't Die - Moonwalker\nby: William Pan (williampan@stanford.edu)\nMarch 28, 2023\n");
}

void print_start_calibration(void) {
  print_calibration_message(SCREEN_X / 2, SCREEN_Y / 2, 15, 0);             // Point to center
  print_calibration_message(15, 15, 15, 1);                                 // Point to top left
  print_calibration_message(SCREEN_X - 15, 15, 15, 2);                      // Point to top right
  print_calibration_message(15, SCREEN_Y - 15, 15, 3);                      // Point to bottom left
  print_calibration_message(SCREEN_X - 15, SCREEN_Y - 15, 15, 4);           // Point to bottom right
}

void print_calibration_message(int x, int y, int r, int i) {
  printf("\nStarting calibration [%d / 5]...\nPoint your arm at the %s...\n", i + 1, strings[i]);
  gl_draw_circle_filled(x, y, r, GL_WHITE);
  gl_copy_buffer();
  timer_delay(1);
  printf("\n[3]");
  timer_delay(1);
  printf("\n[2]");
  timer_delay(1);
  printf("\n[1]\n");
  accel_print_angles();
}

void draw_start_calibration(void) {
  draw_calibration_message(SCREEN_X / 2, SCREEN_Y / 2, 15, 0);             // Point to center
  draw_calibration_message(15, 15, 15, 1);                                 // Point to top left
  draw_calibration_message(SCREEN_X - 15, 15, 15, 2);                      // Point to top right
  draw_calibration_message(15, SCREEN_Y - 15, 15, 3);                      // Point to bottom left
  draw_calibration_message(SCREEN_X - 15, SCREEN_Y - 15, 15, 4);           // Point to bottom right
}

void draw_calibration_message(int x, int y, int r, int i) {
  char buf[1024];
  snprintf(buf, 1024, "Starting calibration [%d / 5]", i + 1);
  gl_draw_string_center(buf, GL_WHITE);
  gl_copy_buffer();
  timer_delay(1);
  gl_draw_string_center(buf, GL_BLUE);
  gl_copy_buffer();

  snprintf(buf, 1024, "Point your arm at the %s", strings[i]);
  gl_draw_string_center(buf, GL_WHITE);
  gl_copy_buffer();
  timer_delay(1);
  gl_draw_string_center(buf, GL_BLUE);
  gl_copy_buffer();
  
  gl_draw_circle_filled(x, y, r, GL_WHITE);
  gl_copy_buffer();

  color_t back = GL_BLUE;
  color_t front = GL_WHITE;
  

  if (i == 0) {
    back = GL_WHITE;
    front = GL_BLUE;
  }

  gl_draw_string_center("3", front);
  gl_copy_buffer();
  timer_delay(1);
  gl_draw_string_center("3", back);
  gl_copy_buffer();

  gl_draw_string_center("2", front);
  gl_copy_buffer();
  timer_delay(1);
  gl_draw_string_center("2", back);
  gl_copy_buffer();

  gl_draw_string_center("1", front);
  gl_copy_buffer();
  timer_delay(1);
  gl_draw_string_center("1", back);
  gl_copy_buffer();

  gl_draw_circle_filled(x, y, r, GL_BLUE);
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