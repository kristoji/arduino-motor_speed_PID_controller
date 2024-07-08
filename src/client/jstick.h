/**************************************************\
 * @file jstick.h                         
 *
 * @brief Header for joystick functions
\**************************************************/
#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/joystick.h>

#define JS_EVENT_SIZE (sizeof(struct js_event))

/**************************************************\
 * @brief Macro for the joystick device
\**************************************************/
#define DEV_JS "/dev/input/js0"
/**************************************************\
 * @brief Macro for abs value of max speed to send
\**************************************************/
#define MAX_VEL 150
/**************************************************\
 * @brief Macro for rounding value of speed
\**************************************************/
#define ROUND_VEL 15

/**************************************************\
 * @brief Represents the target speed to send
 * 
 * @param left_wheel The speed of the left wheel
 * @param right_wheel The speed of the right wheel
\**************************************************/
typedef struct target_speed_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_speed_t;

extern volatile sig_atomic_t keep_running;
extern pthread_mutex_t lock;

extern struct js_event last_event_x;
extern struct js_event last_event_y;

/**************************************************\
 * @brief Opens the serial as non blocking or exits
 * 
 * @param name The name of the serial port
 * @return The file descriptor of the serial port 
\**************************************************/
int check_open(const char* name);
/**************************************************\
 * @brief Thread function to read joystick events
 * 
 * @param arg The file descriptor of the joystick
\**************************************************/
void* read_joystick(void* arg);
/**************************************************\
 * @brief Maps the joystick read values to the wheels
 * 
 * @param x The x value of the joystick
 * @param y The y value of the joystick
 * @param speed The speed to map to
\**************************************************/
void mapJoystickToWheels(int x, int y, target_speed_t * speed);
/**************************************************\
 * @brief Rounds the value to the nearest multiple
 * 
 * @param value The value to round
 * @param round The multiple to round to
 * @return The rounded value 
\**************************************************/
int16_t round_val(int16_t value, int16_t round);
/**************************************************\
 * @brief Sends the speed to the serial port
 * 
 * @param fd The file descriptor of the serial port
 * @param speed The speed to send
\**************************************************/
void send_speed(int fd, target_speed_t* speed);
