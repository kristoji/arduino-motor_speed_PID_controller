/**************************************************\
 * @file h_bridge.c                         
 *
 * @brief Implementation of functions to handle h_bridge
\**************************************************/
#include "h_bridge.h"

void setup_hbridge(uint8_t portb_mask, uint8_t porth_mask)
{
  // setup timer 1
  TCCR1A=TCCR1A_MASK;
  TCCR1B=TCCR1B_MASK;

  // setup timer 2
  TCCR2A=TCCR2A_MASK;
  TCCR2B=TCCR2B_MASK;

	// clear all bits of output compare reg for timer
  OCR1AH=0;OCR1AL=0;
  OCR1BH=0;OCR1BL=0;
  OCR2A=0;
  OCR2B=0;

  // set output
  DDRB |= portb_mask; 
  DDRH |= porth_mask;
}

void control_hbridge(state_t *wheels)
{
  if (wheels[RIGHT_IDX].output_pid > 0)
  {
    OCR1AL = wheels[RIGHT_IDX].output_pid;
    OCR1BL = 0;
  }
  else
  {
    OCR1AL = 0;
    OCR1BL = -wheels[RIGHT_IDX].output_pid;
  }

  if (wheels[LEFT_IDX].output_pid > 0)
  {
    OCR2A = wheels[LEFT_IDX].output_pid;
    OCR2B = 0;
  }
  else
  {
    OCR2A = 0;
    OCR2B = -wheels[LEFT_IDX].output_pid;
  }
}


void print_status_hbridge() 
{
  unsigned char out[1024];
  sprintf((char*) out, 
		"Left Motor:\n"
		"\tpin9 = %u\n\tpin10 = %u\n"
		"Right Motor:\n"
		"\tpin11 = %u\n\tpin12 = %u\n\n",
		(int) OCR2A, (int) OCR2B,
		(int) OCR1AL, (int) OCR1BL
	);
  UART_putString(out);
}