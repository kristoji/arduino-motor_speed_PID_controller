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


#define Kp 0.25
#define Ki 0.125


typedef struct target_speed_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_speed_t;

extern state_t enc[TOTAL_ENCODERS];


int clamp(int value, int max);

void setup_update_timer(void);

void compute_speed(state_t *enc, uint8_t tot_enc);

void update_pid(state_t *enc, uint8_t tot_enc);

void print_status_pid(state_t *enc, uint8_t tot_enc);

void get_target_speed(state_t *enc, target_speed_t *target_speed);

ISR(USART0_RX_vect);