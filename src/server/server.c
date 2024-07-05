#include "server.h"

/*************************************************
 *                GLOBAL VARIABLES               *
 *************************************************/

state_t enc[TOTAL_ENCODERS] = 
{
  {
    {
      .speed = 0,
      .old_cnt = 0,
      .target_speed = 0,
      .integral_err = 0,
      .output = 0
    },
    .counter = 0,
    .prev_value = 0b11,
    .curr_value = 0b11,
    .msk_0 = M_50,
    .msk_1 = M_52
  },
  {
    {
      .speed = 0,
      .old_cnt = 0,
      .target_speed = 0,
      .integral_err = 0,
      .output = 0
    },
    .counter = 0,
    .prev_value = 0b11,
    .curr_value = 0b11,
    .msk_0 = M_53,
    .msk_1 = M_51
  },
};

volatile uint8_t timer_irq = 0;
int writeIndex = 0;
uint8_t g_buf[sizeof(target_speed_t)];
target_speed_t target_speed = {
    .left_wheel = 0,
    .right_wheel = 0
};


/*************************************************
 *          INTERRUPT SERVICE ROUTINES           *
 *************************************************/

ISR(TIMER5_COMPA_vect)
{
  timer_irq = 1;
}

ISR (PCINT0_vect)
{
  update_encoder(enc, TOTAL_ENCODERS);
}

ISR(TIMER3_COMPA_vect)
{
  compute_speed(enc, TOTAL_ENCODERS);
  update_pid(enc, TOTAL_ENCODERS);
  control_hbridge(enc, TOTAL_ENCODERS);
}

// ISR(USART0_RX_vect) 
// {
//   // update_hbridge(UDR0);
  // get_target_speed(enc, TOTAL_ENCODERS, UDR0);
// }
 
ISR(USART0_RX_vect) {
    uint8_t receivedByte = UDR0;
    
    g_buf[writeIndex++] = receivedByte;

    if (writeIndex == sizeof(target_speed_t)) {
        target_speed = *(target_speed_t*)g_buf;
        // uint8_t out[1024];
        // sprintf(out, "left_wheel: %d right_wheel: %d\n", target_speed.left_wheel, target_speed.right_wheel);
        // UART_putString(out);
        memset(g_buf, 0, sizeof(target_speed_t));
        get_target_speed(enc, TOTAL_ENCODERS, &target_speed);
        
        writeIndex = 0;
    }
}


/*************************************************
 *                     MAIN                      *
 *************************************************/


int main(void)
{
  UART_init();
  setup_hbridge(PWMB_MASK, PWMH_MASK);
  setup_encoder(ENC_MASK, TIMER_DELAY);
  setup_pid();

  while(1)
  {
    if (timer_irq)
    {
      timer_irq = 0;
      // print_status_hbridge();
      // print_status_encoder(enc, TOTAL_ENCODERS);
      print_status_pid(enc, TOTAL_ENCODERS);
      UART_putString((uint8_t*)"***********************\n");
    }
    sleep_mode();
  }
}