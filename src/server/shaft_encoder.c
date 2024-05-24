
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include "./avr_common/uart.h" // includes the printf and initializes it
#include <avr/interrupt.h>
#include <avr/sleep.h>

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


#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)

#define MASK 0x0F

#define M_53 (1<<0)
#define M_52 (1<<1)
#define M_51 (1<<2)
#define M_50 (1<<3)

#define E_0 (M_50 | M_52)
#define E_1 (M_53 | M_51)

volatile uint8_t timer_int=0;
volatile uint16_t timer_duration_ms = 1000;


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

typedef struct state_s {
  int counter;

  // val for index 0-4
  uint8_t prev_value;
  uint8_t curr_value;
  uint8_t mask;

  // index of {53-50}
  uint8_t bit0;
  uint8_t bit1;
} state_t;


volatile state_t enc[2] = 
{
  {0, 3, 3, E_0, 3, 1},
  {0, 3, 3, E_1, 0, 2},
};


#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)

void UART_init(void){
  // Set baud rate
  UBRR0H = (uint8_t)(MYUBRR>>8);
  UBRR0L = (uint8_t)MYUBRR;

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8-bit data */ 
  UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);   /* Enable RX and TX */  

}

void UART_putChar(uint8_t c){
  // wait for transmission completed, looping on status bit
  while ( !(UCSR0A & (1<<UDRE0)) );

  // Start transmission
  UDR0 = c;
}

void UART_putString(uint8_t* buf){
  while(*buf){
    UART_putChar(*buf);
    ++buf;
  }
}


ISR (PCINT0_vect)
{
  for (int i = 0; i < 2; i++) 
  {
    enc[i].prev_value = enc[i].curr_value;

    uint8_t rd = PINB & enc[i].mask;
    enc[i].curr_value = (((rd >> (enc[i].bit1-1)) | (rd >> enc[i].bit0)) & 3);
    
    uint8_t idx = ((enc[i].prev_value << 2) | (enc[i].curr_value));
    enc[i].counter += _transition_table[idx];

  }
}

ISR(TIMER5_COMPA_vect)
{
  timer_int = 1;
}

int main(void)
{
  // printf_init(); 
  // printf("\nINIT\n");
  UART_init();
  UART_putString((uint8_t*)"\nINIT\n");

  // PIN 50-53 as input
  // mapped to bits 0-3 of PORTB
  
  // set as input
  DDRB &= ~MASK;
  // enable pull up resistor
  PORTB |= MASK;
  
  // set interrupt on change, looking up PCMSK0
  PCICR |= (1 << PCIE0); 

  TCCR5A = 0;
  TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52);
  OCR5A = (uint16_t)(15.62*timer_duration_ms);

  // clear interrupt
  cli();
  // set PCINT0 to trigger an interrupt on state change 
  PCMSK0 |= MASK;   
  // enable the timer interrupt
  TIMSK5 |= (1 << OCIE5A);
  // enable interrupt
  sei();

  while(1)
  {
    if (timer_int)
    {
      timer_int = 0;

      for (int i = 0; i < 2; i++) 
      {     
        unsigned char out[1024];
        sprintf(out, 
          "encoder: [%d]\n"
          "prev value    = %d%d\n"
          "curr value    = %d%d\n"
          "counter value = %d\n\n", 
          i, 
          (enc[i].prev_value & 0x2) >> 1, enc[i].prev_value & 0x1,
          (enc[i].curr_value & 0x2) >> 1, enc[i].curr_value & 0x1,
          enc[i].counter
        );
        UART_putString(out);
      }
      UART_putString((uint8_t*)"***********************\n");
    }
    sleep_cpu();
    _delay_ms(1000);
  }
  
}
