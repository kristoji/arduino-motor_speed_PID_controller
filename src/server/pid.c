/**************************************************\
 * @file pid.c                         
 *
 * @brief Implementation of functions to control the motors speed
\**************************************************/
#include "pid.h"

/**************************************************\
 * @brief Index to write to the buffer
\**************************************************/
volatile int writeIndex = 0;
/**************************************************\
 * @brief Buffer to store the received data
\**************************************************/
volatile uint8_t g_buf[sizeof(target_delta_t)];
/**************************************************\
 * @brief Stores the received values from the client
\**************************************************/
target_delta_t target_delta_enc = {
    .left_wheel = 0,
    .right_wheel = 0
};

ISR(USART0_RX_vect) 
{
    uint8_t receivedByte = UDR0;
    
    g_buf[writeIndex++] = receivedByte;

    if (writeIndex == sizeof(target_delta_t)) {
        target_delta_enc = *(target_delta_t*)g_buf;
        memset(g_buf, 0, sizeof(target_delta_t));
        set_target_delta_enc(wheels, &target_delta_enc);
        
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

void compute_delta_enc(state_t *wheels, uint8_t tot_wheels)
{
    for (int i = 0; i < tot_wheels; i++) 
    {
        wheels[i].delta_enc = wheels[i].enc.counter - wheels[i].old_cnt;
        wheels[i].old_cnt = wheels[i].enc.counter;
    }
}

void update_pid(state_t *wheels, uint8_t tot_wheels)
{
    for (int i = 0; i < tot_wheels; i++) 
    {
        int error = wheels[i].target_delta_enc - wheels[i].delta_enc;
        error = clamp(error, 100);
        int ref_speed = wheels[i].delta_enc + error;

        wheels[i].integral_err = clamp(wheels[i].integral_err + error, 100);
        wheels[i].output_pid = ref_speed +  Kp * error + Ki * wheels[i].integral_err;

        wheels[i].output_pid = clamp(wheels[i].output_pid, 255);
    }
}

void print_status_pid(state_t *wheels, uint8_t tot_wheels)
{
    for (int i = 0; i < tot_wheels; i++) 
    {
        unsigned char out[1024];
        sprintf((char*) out, 
            "encoder. [%d]\n"
            "speed: %d\n"
            "target speed: %d\n"
            "integral: %d\n\n", 
            i, wheels[i].delta_enc, wheels[i].target_delta_enc, wheels[i].integral_err
        );
        UART_putString(out);
    }
}

void set_target_delta_enc(state_t *wheels, target_delta_t *target_delta_enc)
{
    wheels[RIGHT_IDX].target_delta_enc = target_delta_enc->right_wheel;
    wheels[LEFT_IDX].target_delta_enc = target_delta_enc->left_wheel;
}