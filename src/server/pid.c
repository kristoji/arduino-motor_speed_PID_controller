#include "pid.h"

volatile int writeIndex = 0;
volatile uint8_t g_buf[sizeof(target_speed_t)];
target_speed_t target_speed = {
    .left_wheel = 0,
    .right_wheel = 0
};

ISR(USART0_RX_vect) 
{
    uint8_t receivedByte = UDR0;
    
    g_buf[writeIndex++] = receivedByte;

    if (writeIndex == sizeof(target_speed_t)) {
        target_speed = *(target_speed_t*)g_buf;
        memset(g_buf, 0, sizeof(target_speed_t));
        get_target_speed(enc, &target_speed);
        
        writeIndex = 0;
    }
}


int clamp(int value, int max)
{
    if (value > max)
    {
        return max;
    }
    else if (value < -max)
    {
        return -max;
    }
    return value;
}

void compute_speed(state_t *enc, uint8_t tot_enc)
{
    for (int i = 0; i < tot_enc; i++) 
    {
        // double speed = (double)(enc->counter - enc->old_cnt)/(double)UPDATE_PID_MS;
        enc[i].pid.speed = enc[i].counter - enc[i].pid.old_cnt;
        enc[i].pid.old_cnt = enc[i].counter;
    }
}

void update_pid(state_t *enc, uint8_t tot_enc)
{
    for (int i = 0; i < tot_enc; i++) 
    {
        int error = enc[i].pid.target_speed - enc[i].pid.speed;
        error = clamp(error, 100);
        int ref_speed = enc[i].pid.speed + error;

        enc[i].pid.integral_err = clamp(enc[i].pid.integral_err + error, 100);
        enc[i].pid.output = ref_speed +  Kp * error + Ki * enc[i].pid.integral_err;

        enc[i].pid.output = clamp(enc[i].pid.output, 255);
    }
}

void print_status_pid(state_t *enc, uint8_t tot_enc)
{
    for (int i = 0; i < tot_enc; i++) 
    {
        unsigned char out[1024];
        sprintf((char*) out, 
            "encoder. [%d]\n"
            "speed: %d\n"
            "target speed: %d\n"
            // "output: %d\n\n", 
            "integral: %d\n\n", 
            // i, enc[i].pid.speed, enc[i].pid.target_speed, enc[i].pid.output
            i, enc[i].pid.speed, enc[i].pid.target_speed, enc[i].pid.integral_err
        );
        UART_putString(out);
    }
}

void get_target_speed(state_t *enc, target_speed_t *target_speed)
{

    enc[0].pid.target_speed = target_speed->right_wheel;
    enc[1].pid.target_speed = target_speed->left_wheel;

}