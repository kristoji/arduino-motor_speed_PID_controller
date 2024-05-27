#include "pid.h"

double clamp(double value, double max)
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

// int clamp(int value, int max)
// {
//     if (value > max)
//     {
//         return max;
//     }
//     else if (value < -max)
//     {
//         return -max;
//     }
//     return value;
// }

void setup_pid()
{
    UART_putString("Setting up PID\n");
    
    // set timer
    TCCR3A = 0;
    TCCR3B = TCCR3B_MASK;
    OCR3A = (uint16_t)(15.62*UPDATE_PID_MS);
    
    cli();
    // enable the timer interrupt
    TIMSK3 |= (1 << OCIE3A);
    sei();
}


void compute_speed(state_t *enc)
{
    for (int i = 0; i < TOTAL_ENCODERS; i++) 
    {
        // double speed = (double)(enc->counter - enc->old_cnt)/(double)UPDATE_PID_MS;
        enc[i].speed = enc[i].counter - enc[i].old_cnt;
        enc[i].old_cnt = enc[i].counter;
    }
}

double update_pid(double speed, double target_speed)
{
    static double integral_err = 0;

    double error = target_speed - speed;
    error = clamp(error, 100);
    double ref_speed = target_speed + error;

    double output = 0;

    integral_err = clamp(integral_err + error, 1000);
    output = ref_speed +  Kp * error + Ki * integral_err;

    output = clamp(output, 255);
    return output;
}

void print_status_pid(double speed, double target_speed, double output)
{
    unsigned char out[1024];
    sprintf((char*) out, 
        "speed: %f\n"
        "target speed: %f\n"
        "output: %f\n\n", 
        speed, target_speed, output
    );
    UART_putString(out);
}