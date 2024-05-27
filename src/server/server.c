#include "server.h"

/*************************************************
 *                GLOBAL VARIABLES               *
 *************************************************/

volatile state_t enc[TOTAL_ENCODERS] = 
{
  {0, 0b11, 0b11, M_50, M_52},
  {0, 0b11, 0b11, M_53, M_51},
};

volatile uint8_t timer_irq = 0;


/*************************************************
 *          INTERRUPT SERVICE ROUTINES           *
 *************************************************/

ISR (PCINT0_vect)
{
  for (int i = 0; i < TOTAL_ENCODERS; i++) 
  {
    enc[i].prev_value = enc[i].curr_value;

    enc[i].curr_value = 
      ((PINB & enc[i].msk_1)? 0b10 : 0) | ((PINB & enc[i].msk_0)? 0b01 : 0);
    
    uint8_t idx = ((enc[i].prev_value << 2) | (enc[i].curr_value));
    enc[i].counter += _transition_table[idx];

  }
}

ISR(TIMER5_COMPA_vect)
{
  timer_irq = 1;
}

ISR(USART0_RX_vect) {
    unsigned char received_byte = UDR0;
	update_hbridge(received_byte);
}

/*************************************************
 *                     MAIN                      *
 *************************************************/


int main(void)
{
	UART_init();
	setup_hbridge(PWMB_MASK, PWMH_MASK);
	setup_encoder(ENC_MASK, TIMER_DELAY);

	while(1)
	{
		if (timer_irq)
		{
				timer_irq = 0;
				print_status_hbridge();
				print_status_encoder(enc, TOTAL_ENCODERS);
		}
		sleep_mode();
	}
}