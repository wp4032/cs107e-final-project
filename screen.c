#include "gl.h"
#include "fb.h"
#include "controls.h"
#include "settings.h"
#include "screen.h"
#include "armtimer.h"

static color_t back_color;

static int prev_x = SCREEN_X / 2;
static int prev_y = SCREEN_Y / 2;


// FUNCTION: screen_init
// PARAMS: void
// Initializes the gl / fb with the blue background
void screen_init(void) {
  // Initialize global color variables
  back_color = GL_BLUE;

  // Initializes the graphics library and frame buffer
  gl_init(SCREEN_X, SCREEN_Y, GL_DOUBLEBUFFER);
  screen_clear();
}

// FUNCTION: screen_clear
// PARAMS: void
// Clears the screen to be blue
void screen_clear(void) {
  gl_clear(back_color);
  gl_swap_buffer();
  gl_clear(back_color); 
  gl_swap_buffer();
}


// FUNCTION: screen_loop
// PARAMS: void
// Loops the drawing algorithm on the screen
void screen_loop(void) {
  screen_clear();

  while(1) {
    // Gets the control_action_t struct
    control_action_loop();
    control_action_t ctrl = control_get_action();
    armtimer_disable_interrupts();

    // Draws a line based on the control_action_t instructions
    gl_draw_line(ctrl.x, ctrl.y, prev_x, prev_y, ctrl.color);
    prev_x = ctrl.x;
    prev_y = ctrl.y;
    gl_copy_buffer();
    armtimer_enable_interrupts();
  }
}