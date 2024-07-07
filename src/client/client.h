#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "serial_linux.h"
#include "sample.h"

#define MAX_VEL 150
#define ROUND_VEL 15
#define DEV_JS "/dev/input/js0"

typedef struct speed_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_speed_t;
