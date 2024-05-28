#pragma once

/* From SO Exercise 03-24: 
 *
 *    Double Shaft Encoder in AVR
 *
 * A shaft encoder is a device used to determine the angular 
 * position of a wheel. 
 * Its outputs are two digital signals (A, and B)
 * The "angular position" of the wheel is incremented/decremented 
 * depending on the transitions of the two signals, according to the 
 * following state machine.
 *
 * It uses timers to print each 100ms 
 * the state of the encoder with an interrupt.
 * It uses uart directly instead of 
 * the modified printf
*/

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "./avr_common/uart.h" // includes the printf and initializes it
#include "my_uart.h"
#include "pid.h"

typedef struct state_s {
  pid_t pid;
  int counter;

  // val for index 0-4
  uint8_t prev_value;
  uint8_t curr_value;

  // index of {53-50}
  uint8_t msk_0;
  uint8_t msk_1;
} state_t;


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



void setup_encoder(uint8_t mask, uint16_t timer_duration_ms);

void update_encoder(state_t *enc, int tot_enc);

void print_status_encoder(state_t *enc, int tot_enc);
