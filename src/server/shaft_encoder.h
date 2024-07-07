#pragma once

/*
 * A shaft encoder is a device used to determine the angular 
 * position of a wheel. 
 * Its outputs are two digital signals (A, and B)
 * The "angular position" of the wheel is incremented/decremented 
 * depending on the transitions of the two signals, according to the 
 * following state machine.
 *
*/

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>
// #include "./avr_common/uart.h" // includes the printf and initializes it
#include "my_uart.h"
#include "pid.h"
#include "commons.h"


// PIN 50-53: PORTB's 0-3th bits

// encoder 0
#define ENC_RIGHT_IDX 0
#define M_52 (1<<1)
#define M_50 (1<<3)
// encoder 1
#define ENC_LEFT_IDX 1
#define M_53 (1<<0)
#define M_51 (1<<2)

// total mask to set the pins as input
#define ENC_MASK (M_50 | M_51 | M_52 | M_53)



// ' -> ' is the positive direction of the wheel 
// ' <- ' is the negative direction of the wheel
volatile static const int8_t _transition_table []=
{
      0,  // 00 -> 00
     -1,  // 00 -> 01
      1,  // 00 -> 10
      0,  // 00 -> 11
      1,  // 01 -> 00
      0,  // 01 -> 01
      0,  // 01 -> 10
     -1,  // 01 -> 11
     -1,  // 10 -> 00
      0,  // 10 -> 01
      0,  // 10 -> 10
      1,  // 10 -> 11
      0,  // 11 -> 00
      1,  // 11 -> 01
     -1,  // 11 -> 10
      0   // 11 -> 11
};



void setup_encoder(uint8_t mask);

void update_encoder(state_t *enc, int tot_enc);

void print_status_encoder(state_t *enc, int tot_enc);

ISR (PCINT0_vect);