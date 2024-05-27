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
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "./avr_common/uart.h" // includes the printf and initializes it
#include "my_uart.h"


// Using PINS 11-12 mapped to OC1A-OC1B (need only timer 1)

/******************************************************************************\
 * // fast PWM, non inverted, max 20KHz
 * #define TCCRA_MASK (1<<WGM11)|(1<<WGM10)|(1<<COM1A1)|(1<<COM1B1)
 * // prescaler = 8 -> frequency = 16MHz / 8 / 256 = 7.8KHz
 * #define TCCRB_MASK (1<<CS11)
 * 
 * IT DOES NOT WORK. PIN11 CAN NOT HAVE A DUTY CICLE OF 0 because of `+1`
\******************************************************************************/

// phase-correct PWM, non inverted, max 20KHz
#define TCCRA_MASK (1<<WGM10)|(1<<COM1A1)|(1<<COM1B1)
// prescaler = 8 -> frequency = 16MHz / 8 / 256 / 2 = 3.9KHz
#define TCCRB_MASK (1<<CS11)

#define M_12 1<<6
#define M_11 1<<5

void setup()
{
  UART_init();
  UART_putString((uint8_t*)"\nINIT\n");


  // we will use timer 1
  TCCR1A=TCCRA_MASK;
  TCCR1B=TCCRB_MASK;

	// clear all higher bits of output compare for timer
  OCR1AH=0;
  OCR1AL=0;
  OCR1BH=0;
  OCR1BL=0;

  // set output
  DDRB |= (M_11 | M_12); 
}

void update_PWM(char in)
{
    static short int intensity = 0;

    if (in == 'h' && intensity+8 <= 255)
    {
      intensity += 8;
    }
    else if (in == 'l' && intensity-8 >= -255)
    {
      intensity -= 8;
    }

    if (intensity > 0)
    {
      OCR1AL = intensity;
      OCR1BL = 0;
    }
    else
    {
      OCR1AL = 0;
      OCR1BL = -intensity;
    }
}

char get_input()
{
  char in[1024];
  int len_in = UART_getString(in); 
  
  while(len_in != 3 || (in[0] != 'h' && in[0] != 'l')) 
  {
    UART_putString((uint8_t*)"Usage: \"h\" for higher speed, \"l\" for lower speed\n");
 
    len_in = UART_getString(in);
  }

  return in[0];
}

void print_status() 
{
  unsigned char out[1024];
  sprintf(out, "pin11 = %u\npin12 = %u\n", (int) OCR1AL, (int) OCR1BL);
  UART_putString(out);
}

int main(void)
{
  setup();

  while(1)
  {
    char in = get_input();

    update_PWM(in);
    
    print_status();
    
    _delay_ms(100);
  }
  
}
