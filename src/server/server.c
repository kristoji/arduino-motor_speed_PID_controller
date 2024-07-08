#include "server.h"

int main(void)
{
  UART_init();

  set_sleep_mode(SLEEP_MODE_IDLE);

  setup_update_timer();
  setup_hbridge(PWMB_MASK, PWMH_MASK);
  setup_encoder(ENC_MASK);

  while(1)
  {
    if (timer_irq)
    {
      timer_irq = 0;
      compute_delta_enc(wheels, TOTAL_WHEELS);
      update_pid(wheels, TOTAL_WHEELS);
      update_odometry(&od_status, wheels[LEFT_IDX].delta_enc, wheels[RIGHT_IDX].delta_enc);
      send_odometry(&od_status);
      control_hbridge(wheels);
    }
    sleep_mode();
  }
}  