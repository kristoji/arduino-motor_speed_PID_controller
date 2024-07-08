/**************************************************\
 * @file time.h                         
 *
 * @brief Header for global time functions
\**************************************************/
#pragma once
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

/***********************************************************\
 * @brief Macro to set global timer on CTC mode
 * 
 * @cite 
 * Clear Timer on Compare Match, or CTC. 
 * Instead of counting until an overflow occurs, 
 * the timer compares its count to a value that 
 * was previously stored in a register. 
 * When the count matches that value, 
 * the timer can either set a flag or trigger an interrupt
\***********************************************************/
#define TCCR3B_MASK ((1 << WGM32) | (1 << CS30) | (1 << CS32))
/**************************************************\
 * @brief Represents the delay between updates in ms
\**************************************************/
#define UPDATE_MS 50

/**************************************************\
 * @brief Setups the global timer 
\**************************************************/
void setup_update_timer(void);
/**************************************************\
 * @brief Updates the timer flag on interrupt
\**************************************************/
ISR(TIMER3_COMPA_vect);