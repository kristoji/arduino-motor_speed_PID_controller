/**************************************************\
 * @file client.h                         
 *
 * @brief Header for the main client function
\**************************************************/
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

/**************************************************\
 * @brief Delay between sending control packets in seconds
\**************************************************/
#define DELAY_SEND_CONTROL 1

/**************************************************\
 * @brief Global flag to tell if running 
\**************************************************/
volatile sig_atomic_t keep_running = 1;
/**************************************************\
 * @brief Mutex to protect the joystick data to send
\**************************************************/
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**************************************************\
 * @brief The js mapped speed to send 
\**************************************************/
target_speed_t speed = {
    .left_wheel = 0,
    .right_wheel = 0
};

/**************************************************\
 * @brief Debug flag to print received data
\**************************************************/
extern uint8_t debug;

