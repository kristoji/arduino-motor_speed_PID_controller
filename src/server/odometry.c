#include "odometry.h"


void update_velocity(velocity_t* vel, position_t *pos, int32_t delta_enc_left, int32_t delta_enc_right)
{
    int32_t w_l = ( delta_enc_left / UPDATE_PID_MS ) * ( 2 * PI / TICKS_PER_REV);
    int32_t w_r = ( delta_enc_right / UPDATE_PID_MS ) * ( 2 * PI / TICKS_PER_REV);

    int32_t v_l = w_l * WHEEL_RADIUS;
    int32_t v_r = w_r * WHEEL_RADIUS;

    vel->v = (v_l + v_r) / 2;
    vel->w = (v_r - v_l) / WHEEL_DISTANCE;
}


void update_odometry(position_t *pos, int32_t left, int32_t right);
