#pragma once

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>

#include "my_uart.h"
#include "pid.h"
#include "time.h"

#define WHEEL_RADIUS 4.5
#define WHEEL_DISTANCE 30
#define TICKS_PER_REV 8350
#define DELTA_T (UPDATE_MS/1000.0)
#define TAYLOR_ORD 6

#define PI 3.141592

typedef struct od_status_s {
    float x;
    float y;
    float theta;
    float v;
    float w;
} od_status_t;

void update_odometry(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right);
void update_speed(od_status_t *status, int32_t delta_enc_left, int32_t delta_enc_right);
void print_odometry(od_status_t *status, state_t* enc);
void send_odometry(od_status_t *status);
uint8_t equal(od_status_t *status1, od_status_t *status2);

