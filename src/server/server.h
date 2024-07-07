#pragma once
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom2560.h>        // to avoid error in the IDE
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#include "my_uart.h"
#include "h_bridge.h"
#include "shaft_encoder.h"
#include "pid.h"
#include "odometry.h"
#include "time.h"

extern volatile uint8_t timer_irq;
extern state_t enc[TOTAL_ENCODERS];
extern od_status_t od_status;

