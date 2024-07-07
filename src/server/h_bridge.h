#pragma once

/***********************************************************\
 * Input:
 * - A=1, B=0 -> motor goes forward
 * - A=0, B=1 -> motor goes backward
 *
 * Motor velocity regulation is obtained by applying
 * to these pins a PWM signal with a 20KHz max frequency
 *
\***********************************************************/


/******************************************************************************\
 * https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm/
 * CS set prescaler (according to documentation)
 * 
 * WGM = 011 -> fast PWM: 0-255
 * frequency = 16MHz / prescaler / 256
 * duty cycle = OCset+1 / 256
 * 
 * WGM = 001 -> phase-correct PWM: 0-255-0
 * frequency = 16MHz / prescaler / 256 / 2
 * duty cycle = OCset / 255
 * 
 * WGM = 111 -> fast PWM with OCRA controlling the top limit: 0-OCRA
 * frequency = 16MHz / prescaler / OCRA+1 
 * duty cycle = OCset+1 / OCRA+1
 * 
 * WGM = 101 -> phase-correct PWM with OCRA controlling the top limit: 0-OCRA-0
 * frequency = 16MHz / prescaler / OCRA / 2
 * duty cycle = OCset / OCRA
\******************************************************************************/
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "my_uart.h"
#include "shaft_encoder.h"


/*************************************************
 *                  PWM MACROS                   *
 *************************************************/

// phase-correct PWM, non inverted, max 20KHz
#define TCCR1A_MASK (1<<WGM10)|(1<<COM1A1)|(1<<COM1B1)
#define TCCR2A_MASK (1<<WGM20)|(1<<COM2A1)|(1<<COM2B1)
// prescaler = 8 -> frequency = 16MHz / 8 / 256 / 2 = 3.9KHz
#define TCCR1B_MASK (1<<CS11)
#define TCCR2B_MASK (1<<CS21)


// PIN 9-12: PORTH's 6th bit, PORTB's 4-6th bits

// PORTB
#define M_12 1<<6
#define M_11 1<<5

#define M_10 1<<4
// PORTH
#define M_09 1<<6

#define PWMB_MASK (M_12 | M_11 | M_10)
#define PWMH_MASK (M_09)


void setup_hbridge(uint8_t portb_mask, uint8_t porth_mask);

void update_hbridge(char in);

void control_hbridge(state_t *enc, uint8_t tot_enc);

void print_status_hbridge(void);
