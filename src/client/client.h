#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "serial_linux.h"

#define MAX_VEL 50
#define DEV_JS "/dev/input/js0"

typedef struct speed_s {
    int8_t left_wheel;
    int8_t right_wheel;
} target_speed_t;
