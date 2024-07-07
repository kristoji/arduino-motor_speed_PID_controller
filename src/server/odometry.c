#include "odometry.h"

od_status_t od_status = {
    .x = 0,
    .y = 0,
    .theta = 0,
    .v = 0,
    .w = 0
}; 

void update_velocity(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right)
{
    float w_l = ( delta_enc_left / (float) DELTA_T ) * ( 2 * PI / (float) TICKS_PER_REV);
    float w_r = ( delta_enc_right / (float) DELTA_T ) * ( 2 * PI / (float) TICKS_PER_REV);

    float v_l = w_l * WHEEL_RADIUS;
    float v_r = w_r * WHEEL_RADIUS;

    status->v = (v_l + v_r) / 2;
    status->w = (v_r - v_l) / WHEEL_DISTANCE;    
}

void update_odometry(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right)
{
    update_velocity(status, delta_enc_left, delta_enc_right);

    float delta_ro = status->v * DELTA_T;
    float delta_theta = status->w * DELTA_T;

    float delta_x = delta_ro * cos(status->theta);
    float delta_y = delta_ro * sin(status->theta);
 
    status->x += delta_x;
    status->y += delta_y;
    status->theta += delta_theta;
}


void print_odometry(od_status_t *status, state_t* enc)
{
    uint8_t out[1024];
    sprintf(out, "x: %f, y: %f, theta: %f, v: %f, w: %f\n\0", status->x, status->y, status->theta, status->v, status->w);
    UART_putString(out);
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