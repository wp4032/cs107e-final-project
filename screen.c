#include "gl.h"
#include "fb.h"
#include "controls.h"
#include "settings.h"
#include "screen.h"

static color_t back_color;

static int prev_x = SCREEN_X / 2;
static int prev_y = SCREEN_Y / 2;

void screen_init(void) {
  // Initialize global color variables
  back_color = GL_BLUE;

  // Initializes the graphics library and frame buffer
  gl_init(SCREEN_X, SCREEN_Y, GL_DOUBLEBUFFER);
  screen_clear();
}

void screen_clear(void) {
  gl_clear(back_color);
  gl_swap_buffer();
  gl_clear(back_color); 
  gl_swap_buffer();
}

void loop_test(void) {
  screen_clear();

  while(1) {
    loop();
    control_action_t ctrl = control_get_action();
    gl_draw_pixel(ctrl.x, ctrl.y, GL_WHITE);
    gl_swap_buffer();
    // gl_copy_buffer();
  }
}