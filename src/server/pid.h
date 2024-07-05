#pragma once

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>


#include "./avr_common/uart.h" 
#include "my_uart.h"

/***********************************************************\
 * Clear Timer on Compare Match, or CTC. 
 * Instead of counting until an overflow occurs, 
 * the timer compares its count to a value that 
 * was previously stored in a register. 
 * When the count matches that value, 
 * the timer can either set a flag or trigger an interrupt
\***********************************************************/
#define TCCR3B_MASK ((1 << WGM32) | (1 << CS30) | (1 << CS32))

#define UPDATE_PID_MS 50

#define Kp 0.25
#define Ki 0.125

typedef struct pid_s
{
    int speed;
    int old_cnt;
    int target_speed;
    int integral_err;
    int output;
} pid_t;

typedef struct speed_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_speed_t;

// forward declaration
typedef struct state_s state_t;


int clamp(int value, int max);

void setup_pid(void);

void compute_speed(state_t *enc, uint8_t tot_enc);

void update_pid(state_t *enc, uint8_t tot_enc);

void print_status_pid(state_t *enc, uint8_t tot_enc);

void get_target_speed(state_t *enc, uint8_t tot_enc, target_speed_t *target_speed);


#include "shaft_encoder.h"