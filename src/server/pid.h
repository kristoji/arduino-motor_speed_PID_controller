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


typedef struct target_delta_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_delta_t;

extern state_t wheels[TOTAL_WHEELS];


int clamp(int value, int max);

void setup_update_timer(void);

void compute_delta_enc(state_t *wheels, uint8_t tot_wheels);

void update_pid(state_t *wheels, uint8_t tot_wheels);

void print_status_pid(state_t *wheels, uint8_t tot_wheels);

void set_target_delta_enc(state_t *wheels, target_delta_t *target_delta_enc);

ISR(USART0_RX_vect);