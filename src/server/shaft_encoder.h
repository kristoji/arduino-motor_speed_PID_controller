/**************************************************\
 * @file shaft_encoder.h                         
 *
 * @brief Header for encoder functions
\**************************************************/
#pragma once
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "my_uart.h"
#include "pid.h"
#include "commons.h"


// PIN 50-53: PORTB's 0-3th bits


/**************************************************\
 * @brief Encoder right A pin mask
\**************************************************/
#define M_52 (1<<1)
/**************************************************\
 * @brief Encoder right B pin mask
\**************************************************/
#define M_50 (1<<3)
/**************************************************\
 * @brief Encoder left A pin mask
\**************************************************/
#define M_53 (1<<0)
/**************************************************\
 * @brief Encoder left B pin mask
\**************************************************/
#define M_51 (1<<2)

/**************************************************\
 * @brief Encoder total mask
\**************************************************/
#define ENC_MASK (M_50 | M_51 | M_52 | M_53)



/**************************************************\
 * @brief transition table for the encoder
 * 
 * @cite 
 * ' -> ' is the positive direction of the wheel 
 * ' <- ' is the negative direction of the wheel
\**************************************************/
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



/**************************************************\
 * @brief Setups the encoder pins and interrupts
 * 
 * @param mask The mask of the encoder
\**************************************************/
void setup_encoder(uint8_t mask);

/**************************************************\
 * @brief Updates the encoder values
 * 
 * @param wheels The wheels to update
 * @param tot_wheels The total number of wheels
\**************************************************/
void update_encoder(state_t *wheels, uint8_t tot_wheels);

/**************************************************\
 * @brief Prints the encoder values
 * 
 * @param wheels The wheels to print
 * @param tot_wheels The total number of wheels
\**************************************************/
void print_status_encoder(state_t *wheels, uint8_t tot_wheels);

/**************************************************\
 * @brief ISR to update the encoder counter on change
\**************************************************/
ISR (PCINT0_vect);