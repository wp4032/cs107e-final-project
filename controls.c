#include "accel.h"
#include "interrupts.h"
#include "messages.h"
#include "printf.h"
#include "timer.h"
#include "peripherals.h"
#include "controls.h"
#include "settings.h"
#include "math.h"
#include "armtimer.h"
#include "interrupts.h"

// Variables for the offset in the accelerometer and gyroscope
// Used to zero out any potential misalignments from the housing itself
static signed short accel_x_offset = 0;
static signed short accel_y_offset = 0;
static signed short accel_z_offset = 0;
static signed short gyro_x_offset = 0;
static signed short gyro_y_offset = 0;
static signed short gyro_z_offset = 0;

// Variables for the pitch angle when calibrating. We use these pitch angles
// to then know how we move up and down the screen
static float flat_pitch = 0.0;
static float top_left_pitch = 0.0;
static float top_right_pitch = 0.0;
static float bottom_left_pitch = 0.0;
static float bottom_right_pitch = 0.0;
static float average_top_pitch = 0.0;
static float average_bottom_pitch = 0.0;

// Global accel, gyro, and angles structs to get the values from helper functions
control_accel_t accel;
control_gyro_t gyro;
control_angles_t angles;

// Global limits and control action structs that instruct what the controls are
static control_limits_t limits;
static control_action_t control;


// Helper function for arm timer interrupts
static void handle_controls(unsigned int pc, void *aux_data);


// FUNCTION: controls_init
// PARAMS: void
// RETURNS: initializes the controls module by:
// (1) setting the max and minimum limits
// (2) setting the initial location of the cursor (center of screen)
// (3) setting the sensitivity and threshold of activation of the control
// (4) intializes current measurement and starts calibration process
void controls_init(void) {
  // Enables the armtimer interrupts
  armtimer_init(30000);
  armtimer_enable();
  armtimer_enable_interrupts();
  interrupts_register_handler(INTERRUPTS_BASIC_ARM_TIMER_IRQ, handle_controls, NULL);
  interrupts_enable_source(INTERRUPTS_BASIC_ARM_TIMER_IRQ);

  set_limits(SCREEN_X, SCREEN_Y);
  set_location(SCREEN_X, SCREEN_Y);
  set_sensitivity_threshold(SENSITIVITY, THRESHOLD);
  calibrate();
}


// FUNCTION: set_limits
// PARAMS: signed int screen_x, signed int screen_y
// RETURNS: sets the minimum, maximum in x and y coordinates of the controls
void set_limits(signed int screen_x, signed int screen_y) {
  limits.min_x = 0;
  limits.max_x = screen_x;
  limits.min_y = 0;
  limits.max_y = screen_y;
  control.limits = limits;
}


// FUNCTION: set_location
// PARAMS: signed int screen_x, signed int screen_y
// RETURNS: sets the starting place of the controller (this case center of screen)
void set_location(signed int screen_x, signed int screen_y) {
  control.x = screen_x / 2;
  control.y = screen_y / 2;
  control.action = EMG_INACTIVE;      // Wasn't able to implement because EMG sensor didn't work; but when EMG would be active, this would draw on the screen
  control.color = 0xffffffff;
}


// FUNCTION: set_sensitivity_threshold
// PARAMS: signed int sensitivity, signed int threshold
// RETURNS: sets the sensitivity (how responsive the device is) and the threshold
// (when the device begins to activate)
void set_sensitivity_threshold(signed int sensitivity, signed int threshold) {
  control.sensitivity = sensitivity;
  control.threshold = threshold;
}


// FUNCITON: calibrate_flat
// PARAMS: void
// RETURNS: starts the calibration for when pointing straight at the screen;
// it will clear the accelerometer and gyroscope to get its offsets and if
// the device is not flat, will ask user to restart process again.
static void calibrate_flat(void) {
  int attempts = 0;

  int result = clear_accel() || clear_gyro();

  if (result) return;

  while (result) {
    if (attempts == 2) {
      draw_calibration_failure();
      error_led();
      return;
    }
    draw_calibration_try_again();
    attempts++;
  }
}

// FUNCTION: calibrate
// PARAMS: void
// RETURNS: starts a calibration process that helps intialize its beginning position; it will ask user
// to point the device towards the screen and an error will illuminate if calibration doesn't work
void calibrate(void) {
  draw_calibration_message(SCREEN_X / 2, SCREEN_Y / 2, 15, 0);             // Point to center
  calibrate_flat();                                                        // Clears the gyroscope and accelerometer to be flat
  flat_pitch = angles.pitch_x;

  draw_calibration_message(15, 15, 15, 1);                                 // Point to top left
  top_left_pitch = angles.pitch_x;

  draw_calibration_message(SCREEN_X - 15, 15, 15, 2);                      // Point to top right
  top_right_pitch = angles.pitch_x;

  draw_calibration_message(15, SCREEN_Y - 15, 15, 3);                      // Point to bottom left
  bottom_left_pitch = angles.pitch_x;

  draw_calibration_message(SCREEN_X - 15, SCREEN_Y - 15, 15, 4);           // Point to bottom right
  bottom_right_pitch = angles.pitch_x;

  average_top_pitch = (top_left_pitch + top_right_pitch) / 2.0;            // Averages out the pitch angles to be more precise
  average_bottom_pitch = (bottom_left_pitch + bottom_right_pitch) / 2.0;
  
  draw_calibration_success();
}


// FUNCTION: clear_accel
// PARAMS: void
// RETURNS: clears the current acceleration in the accelerometer and sets that orientation as the base point
// Only call when in the center 
int clear_accel(void) {
  accel_x_offset = accel.accel_x;
  accel_y_offset = accel.accel_y;
  accel_z_offset = accel.accel_z;

  // If orientation is not flat in the y direction or the z direction --> will return false
  // Acceleration in z is always gravity (980 mg)
  // X orientation has some more tolerance 
  accel_z_offset -= 980;
  return (abs(accel_x_offset) > 250 || abs(accel_y_offset) > 100 || abs(accel_z_offset) > 100);
}


// FUNCTION: clear_gyro
// PARAMS: void
// RETURNS: clears the current gyroscope measurements in the accelerometer and sets that orientation as the base point
int clear_gyro(void) {
  gyro_x_offset = gyro.gyro_x;
  gyro_y_offset = gyro.gyro_y;
  gyro_z_offset = gyro.gyro_z;
  // If angular rotation in x, y, z direction is not still --> will return false
  return (abs(gyro_x_offset) > 50 || abs(gyro_y_offset) > 50 || abs(gyro_z_offset) > 50);
}


// FUNCTION: control_read_accel
// PARAMS: void
// RETURNS: gets a reading of the accelerometer and returns a control_accel_t struct
void control_read_accel(void) {
  accel.accel_x = (accel_get_x() - accel_x_offset)/16;
  accel.accel_y = (accel_get_y() - accel_y_offset)/16; 
  accel.accel_z = (accel_get_z() - accel_z_offset)/16;

  // Uncomment below to visualize the current acceleration
  // printf("accel=(%d,%d,%d)\n", accel.accel_x, accel.accel_y, accel.accel_z);
}


// FUNCTION: control_read_gyro
// PARAMS: void
// RETURNS: gets a reading of the gyroscope and returns a control_gyro_t struct
void control_read_gyro(void) {
  gyro.gyro_x = (gyro_get_x() - gyro_x_offset)/16;
  gyro.gyro_y = (gyro_get_y() - gyro_y_offset)/16; 
  gyro.gyro_z = (gyro_get_z() - gyro_z_offset)/16;

  // Uncomment below to visualize the current angular velocity
  // printf("gyro=(%d,%d,%d)\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
}

// FUNCTION: control_read_angles
// PARAMS: void
// RETURNS: gets a reading the pitch and roll angles from the accelerometer
void control_read_angles(void) {
  accel_get_angles(&(angles.pitch_x), &(angles.roll_y));

  // Uncomment below to visualize the pitch and roll angles
  // printf("pitch_x: %d, roll_y: %d\n", (int) (angles.pitch_x * 1000), (int) (angles.roll_y * 1000));
}


// FUNCTION: control_read_action
// PARAMS: control_action_t *ctrl
// RETURNS: turns the accelerometer and gyroscope measurements into motion in the x,y plane
// NOTE: for us, the x and y frame will be in the y and z frame of the accelerometer respectively
void control_read_action(control_action_t *ctrl) {
  // Sees if the current acceleration is above the threshold acceleration; if so, will get an adjusted 
  // x, y movement based on sensitivity.
  signed int delta_y = abs(accel.accel_y) > ctrl->threshold ? accel.accel_y / ctrl->sensitivity : 0;

  // Checks if adding the delta_y and delta_z with the current x, y position will be within bounds 
  ctrl->x += (delta_y + ctrl->x < ctrl->limits.max_x && delta_y + ctrl->x > ctrl->limits.min_x) ? delta_y : 0; 

  // Calculates the current y value on the screen by interpolating between the top and bottom pitch angles
  if (angles.pitch_x > average_bottom_pitch && angles.pitch_x < average_top_pitch) {
    switch(angles.pitch_x > flat_pitch) {
      case 1:
        ctrl->y = ((angles.pitch_x - flat_pitch) / (average_top_pitch - flat_pitch)) * (SCREEN_Y / 2) + (SCREEN_Y / 2);
      case 0:
        ctrl->y = ((flat_pitch - angles.pitch_x) / (flat_pitch - average_bottom_pitch)) * (SCREEN_Y / 2) + (SCREEN_Y / 2);
    }
  }

  // Changes the color of the cursor by how much you turn your wrist
  ctrl->color = (unsigned int) (0xffffffff * (angles.roll_y / 180.0));
}


// FUNCTION: control_get_action
// PARAMS: void
// RETURNS: returns the current control_action_t struct for other programs
// such as the screen program to interface with
control_action_t control_get_action(void) {
  return control;
}


// FUNCTION: control_action_loop
// PARAMS: void
// RETURNS: calls an iteration in getting the control_action_t struct by
// reading blinking peripheral LEDs and getting the control_action_t struct
void control_action_loop(void) {
  armtimer_disable_interrupts();
  success_led_on();
  control_read_action(&control);
  success_led_off();
  armtimer_enable_interrupts();
}


// FUNCTION: handle_controls
// PARAMS: unsigned int pc, void *aux_data
// RETURNS: uses armtimer interrupts to get the readings in the accelerometer, gyroscope,
// and angles. The data gathering must need to happen at all times because the complementary
// filter needs to integrate with time to get the angular velocity.
static void handle_controls(unsigned int pc, void *aux_data) {
  if (armtimer_check_and_clear_interrupt()) {
    control_read_accel();
    control_read_gyro();
    control_read_angles();
  }
}