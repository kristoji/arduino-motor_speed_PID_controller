/**************************************************\
 * @file pid.h                         
 *
 * @brief Header for pid control functions
\**************************************************/
#pragma once

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <string.h>

#include "my_uart.h"
#include "commons.h"


/**************************************************\
 * @brief Proportional const for the PID controller
\**************************************************/
#define Kp 0.25
/**************************************************\
 * @brief Integral const for the PID controller
\**************************************************/
#define Ki 0.125


/**************************************************\
 * @brief Represents the speed to receive from client
 * 
 * @param left_wheel The speed of the left wheel
 * @param right_wheel The speed of the right wheel
\**************************************************/
typedef struct target_delta_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_delta_t;

extern state_t wheels[TOTAL_WHEELS];


/**************************************************\
 * @brief Clamp the value to the max
 * 
 * @param value The value to clamp
 * @param max The maximum abs value to clamp to
\**************************************************/
int clamp(int value, int max);

/**************************************************\
 * @brief Sets the update timer up
\**************************************************/
void setup_update_timer(void);

/**************************************************\
 * @brief Computes the delta encoder value
 * 
 * @param wheels The wheels to compute the delta for
 * @param tot_wheels The total number of wheels
\**************************************************/
void compute_delta_enc(state_t *wheels, uint8_t tot_wheels);

/**************************************************\
 * @brief Computes and updates the pid values
 * 
 * @param wheels The wheels to compute the pid for
 * @param tot_wheels The total number of wheels
\**************************************************/
void update_pid(state_t *wheels, uint8_t tot_wheels);

/**************************************************\
 * @brief Prints the pid status
 * 
 * @param wheels The wheels to print
 * @param tot_wheels The total number of wheels
\**************************************************/
void print_status_pid(state_t *wheels, uint8_t tot_wheels);

/**************************************************\
 * @brief Set the target delta encoder value 
 * 
 * @param wheels The wheels to update
 * @param target_delta_enc The target delta encoder value to set
\**************************************************/
void set_target_delta_enc(state_t *wheels, target_delta_t *target_delta_enc);

/**************************************************\
 * @brief Receives the target delta encoder value from the  client
\**************************************************/
ISR(USART0_RX_vect);