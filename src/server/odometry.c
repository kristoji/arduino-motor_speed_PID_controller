/**************************************************\
 * @file odometry.c                         
 *
 * @brief Implementation of functions to compute and send odometry
\**************************************************/
#include "odometry.h"

/**************************************************\
 * @brief current odometry values of the robot
\**************************************************/
od_status_t od_status = {
    .x = 0,
    .y = 0,
    .theta = 0,
    .v = 0,
    .w = 0
}; 
/**************************************************\
 * @brief Previous odometry values of the robot
\**************************************************/
od_status_t od_status_old = {
    .x = -1,
    .y = 1,
    .theta = 0,
    .v = 0,
    .w = 0
}; 

void update_speed(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right)
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
    update_speed(status, delta_enc_left, delta_enc_right);

    float delta_ro = status->v * DELTA_T;
    float delta_theta = status->w * DELTA_T;

    float delta_x = delta_ro * cos(status->theta);
    float delta_y = delta_ro * sin(status->theta);
 
    status->x += delta_x;
    status->y += delta_y;
    status->theta += delta_theta;
}


void print_odometry(od_status_t *status)
{
    uint8_t out[1024];
    sprintf(out, "x: %f, y: %f, theta: %f, v: %f, w: %f\n\0", status->x, status->y, status->theta, status->v, status->w);
    UART_putString(out);
}

uint8_t equal(od_status_t *status1, od_status_t *status2)
{
    return (status1->x == status2->x) && (status1->y == status2->y) && (status1->theta == status2->theta) && (status1->v == status2->v) && (status1->w == status2->w);
}

void send_odometry(od_status_t *status)
{
    if (!equal(status, &od_status_old))
    {
        od_status_old = *status;
        uint8_t* p = (uint8_t*) status;

        for(uint8_t i = 0; i < sizeof(od_status_t); ++i)
        {
            UART_putChar(*p);
            ++p;
        }
    }
} 