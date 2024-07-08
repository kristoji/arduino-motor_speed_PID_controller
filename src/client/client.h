#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/joystick.h>

#include "serial_linux.h"
#include "sample.h"
#include "jstick.h"

#define DELAY_SEND_CONTROL 1

volatile sig_atomic_t keep_running = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

target_speed_t speed = {
    .left_wheel = 0,
    .right_wheel = 0
};

extern uint8_t debug;

