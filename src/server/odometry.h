/**************************************************\
 * @file odometry.h                         
 *
 * @brief Header for odometry functions
\**************************************************/
#pragma once

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>

#include "my_uart.h"
#include "pid.h"
#include "time.h"

/**************************************************\
 * @brief Wheels radius in cm
\**************************************************/
#define WHEEL_RADIUS 4.5
/**************************************************\
 * @brief Wheels distance in cm
\**************************************************/
#define WHEEL_DISTANCE 30
/**************************************************\
 * @brief Encoder ticks per revolution
\**************************************************/
#define TICKS_PER_REV 8350
/**************************************************\
 * @brief Update time in seconds
\**************************************************/
#define DELTA_T (UPDATE_MS/1000.0)

/**************************************************\
 * @brief PI constant
\**************************************************/
#define PI 3.14159265359

/**************************************************\
 * @brief Represents the odometry status
 * 
 * @param x The x position
 * @param y The y position
 * @param theta The angle
 * @param v The linear velocity
 * @param w The angular velocity
\**************************************************/
typedef struct od_status_s {
    float x;
    float y;
    float theta;
    float v;
    float w;
} od_status_t;

/**************************************************\
 * @brief Updates the odometry values 
 * 
 * @param status The odometry status to update
 * @param delta_enc_left The left encoder delta
 * @param delta_enc_right The right encoder delta
\**************************************************/
void update_odometry(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right);
/**************************************************\
 * @brief Computes the speed values 
 * 
 * @param status The odometry status to update
 * @param delta_enc_left The left encoder delta
 * @param delta_enc_right The right encoder delta
\**************************************************/
void update_speed(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right);
/**************************************************\
 * @brief Prints the odometry values over the serial 
 * 
 * @param status The odometry status to print
\**************************************************/
void print_odometry(od_status_t *status);
/**************************************************\
 * @brief Sends the odometry values over the serial
 *  if they are different from the previous values 
 * 
 * @param status The odometry status to send
\**************************************************/
void send_odometry(od_status_t *status);
/**************************************************\
 * @brief Checks if two odometry values are equal 
 * 
 * @param status1 The first odometry status
 * @param status2 The second odometry status
 * @return 1 if equal, 0 otherwise
\**************************************************/
uint8_t equal(od_status_t *status1, od_status_t *status2);

