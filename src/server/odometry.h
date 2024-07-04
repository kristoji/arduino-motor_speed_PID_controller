#pragma once

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "pid.h"

#define WHEEL_RADIUS 0.045
#define WHEEL_DISTANCE 0.3
#define TICKS_PER_REV 1024      // TODO: check this value

#define PI 3.141592

typedef struct position_s {
    int32_t x;
    int32_t y;
    int32_t theta;
} position_t;

typedef struct velocity_s {
    int32_t v;
    int32_t w;
} velocity_t;

void update_velocity(velocity_t* vel, position_t *pos, int32_t delta_enc_left, int32_t delta_enc_right);
void update_odometry(position_t *pos, int32_t left, int32_t right);

