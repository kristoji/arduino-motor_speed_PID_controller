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
      compute_speed(enc, TOTAL_ENCODERS);
      update_pid(enc, TOTAL_ENCODERS);
      update_odometry(&od_status, enc[1].pid.speed, enc[0].pid.speed);
      send_odometry(&od_status);
      control_hbridge(enc, TOTAL_ENCODERS);
    }
    sleep_mode();
  }
}  