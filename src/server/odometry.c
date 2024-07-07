#include "odometry.h"


void update_velocity(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right)
{
    float w_l = ( delta_enc_left / (float) DELTA_T ) * ( 2 * PI / (float) TICKS_PER_REV);
    float w_r = ( delta_enc_right / (float) DELTA_T ) * ( 2 * PI / (float) TICKS_PER_REV);

    float v_l = w_l * WHEEL_RADIUS;
    float v_r = w_r * WHEEL_RADIUS;

    status->v = (v_l + v_r) / 2;
    status->w = (v_r - v_l) / WHEEL_DISTANCE;

    // uint8_t out[1024];
    // sprintf(out, "v_l: %d, v_r: %d, w_l: %d, w_r: %d\n", v_l, v_r, w_l, w_r);
    // UART_putString(out);
    
}

float taylor_c(float theta)
{
    float res = 0;
    float theta_2 = (theta * theta);
    float curr_theta = theta;
    float succ_factorial = 1;

    for (int i = 1; i < TAYLOR_ORD; i++)
    {
        succ_factorial *= (i+1);

        if (i % 2 != 0)
        {
            float add = (i % 4 == 1)? curr_theta : -curr_theta;
            res += add / succ_factorial;
            curr_theta *= theta_2;
        }
    }
    return res;
}

float taylor_s(float theta)
{
    float res = 1;
    float theta_2 = (theta * theta);
    float curr_theta = theta_2;
    float succ_factorial = 2;

    for (int i = 2; i < TAYLOR_ORD; i++)
    {
        succ_factorial *= (i+1);

        if (i % 2 == 0)
        {
            float add = (i % 4 == 0)? curr_theta : -curr_theta;
            res += add / succ_factorial;
            curr_theta *= theta_2;
        }
    }
    return res;
}

void update_odometry(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right)
{
    // uint8_t out[1024];
    // sprintf(out, "delta_enc_left: %d, delta_enc_right: %d\n", delta_enc_left, delta_enc_right);
    // UART_putString(out);

    update_velocity(status, delta_enc_left, delta_enc_right);

    float delta_ro = status->v * DELTA_T;
    float delta_theta = status->w * DELTA_T;

    float delta_x = delta_ro * cos(status->theta);
    float delta_y = delta_ro * sin(status->theta);
    // float delta_x = delta_ro * taylor_s(status->theta);
    // float delta_y = delta_ro * taylor_c(status->theta);

    status->x += delta_x;
    status->y += delta_y;
    status->theta += delta_theta;
}


void print_odometry(od_status_t *status, state_t* enc)
{
    uint8_t out[1024];
    sprintf(out, "x: %f, y: %f, theta: %f, v: %f, w: %f\n\0", status->x, status->y, status->theta, status->v, status->w);
    UART_putString(out);

    // sprintf(out, "left: %d, right: %d\n\0", enc[1].pid.speed, enc[0].pid.speed);
    // UART_putString(out);

    // float w_l = ( enc[1].pid.speed / DELTA_T ) * ( 2 * PI / TICKS_PER_REV);
    // float w_r = ( enc[0].pid.speed / DELTA_T ) * ( 2 * PI / TICKS_PER_REV);

    // sprintf(out, "first: %f, second: %f\n\0", enc[1].pid.speed / DELTA_T,  2 * PI / TICKS_PER_REV);
    // UART_putString(out);


    // sprintf(out, "w_l: %f, w_r: %f\n\0", w_l, w_r);
    // UART_putString(out);

    // float v_l = w_l * WHEEL_RADIUS;
    // float v_r = w_r * WHEEL_RADIUS;

    // sprintf(out, "v_l: %f, v_r: %f\n\0", v_l, v_r);
    // UART_putString(out);

    // status->v = (v_l + v_r) / 2;
    // status->w = (v_r - v_l) / WHEEL_DISTANCE;

    // sprintf(out, "v: %f, w: %f\n\0", status->v, status->w);
    // UART_putString(out);

}

void send_odometry(od_status_t *status)
{
    uint8_t* p = (uint8_t*) status;

    for(uint8_t i = 0; i < sizeof(od_status_t); ++i)
    {
        UART_putChar(*p);
        ++p;
    }
} 