#pragma once
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

#define JS_EVENT_SIZE (sizeof(struct js_event))
#define DEV_JS "/dev/input/js0"
#define MAX_VEL 150
#define ROUND_VEL 15

typedef struct target_speed_s {
    int16_t left_wheel;
    int16_t right_wheel;
} target_speed_t;

extern volatile sig_atomic_t keep_running;
extern pthread_mutex_t lock;

extern struct js_event last_event_x;
extern struct js_event last_event_y;

int check_open(const char* name);
void* read_joystick(void* arg);
void mapJoystickToWheels(int x, int y, target_speed_t * speed);
int16_t round_val(int16_t value, int16_t round);
void send_speed(int fd, target_speed_t* speed);
