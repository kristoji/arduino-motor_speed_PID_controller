#pragma once
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#include "./avr_common/uart.h" // includes the printf and initializes it
#include "my_uart.h"
#include "h_bridge.h"
#include "shaft_encoder.h"
#include "pid.h"


/*************************************************
 *                  PWM MACROS                   *
 *************************************************/

// PIN 9-12: PORTH's 6th bit, PORTB's 4-6th bits

// PORTB
#define M_12 1<<6
#define M_11 1<<5

#define M_10 1<<4
// PORTH
#define M_09 1<<6

#define PWMB_MASK (M_12 | M_11 | M_10)
#define PWMH_MASK (M_09)


/*************************************************
 *                  ENCODER MACROS               *
 *************************************************/
#define TOTAL_ENCODERS 2

// PIN 50-53: PORTB's 0-3th bits

// encoder 0
#define M_52 (1<<1)
#define M_50 (1<<3)
// encoder 1
#define M_53 (1<<0)
#define M_51 (1<<2)

// total mask to set the pins as input
#define ENC_MASK (M_50 | M_51 | M_52 | M_53)


/*************************************************
 *                     OTHERS                    *
 *************************************************/

#define TIMER_DELAY 1000



