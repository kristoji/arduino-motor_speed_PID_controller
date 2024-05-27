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
#include "shaft_encoder.h"
#include "server.h"

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

#define Kp 1
#define Ki 1

double clamp(double value, double max);
// int clamp(int value, int max);

void setup_pid(void);

void compute_speed(state_t *enc);

double update_pid(double speed, double target_speed);

void print_status_pid(double speed, double target_speed, double output);