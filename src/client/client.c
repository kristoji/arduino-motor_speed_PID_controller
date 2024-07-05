#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/joystick.h>
#include <pthread.h>
#include <signal.h>

void send_speed(int fd, target_speed_t* speed)
{
    uint8_t sent_bytes = 0;
    uint8_t* ptr = (uint8_t*) speed;
    
    do
    {
        int n = write(fd, ptr + sent_bytes, sizeof(target_speed_t) - sent_bytes);
        sent_bytes += n;
    } while (sent_bytes < sizeof(target_speed_t));
    

    // printf("Settings sent\n");
}

int16_t round_val(int16_t value, int16_t round)
{
    int16_t remainder = value % round;
    // printf("Value: %d, Remainder: %d\n", value, remainder);
    if (value > 0 && remainder >= (round / 2)+1)
    {
        value += round;
    }
    else if (value < 0 && -remainder >= (round / 2)+1)
    {
        value -= round;
    }
    return  ((int16_t)(value / round) * round);
}

void mapJoystickToWheels(int x, int y, target_speed_t * speed) {
    // Normalize joystick values to the range -1 to 1
    float norm_x = x / 32767.0;
    float norm_y = y / 32767.0;

    // Compute the velocity for each wheel
    speed->left_wheel = (int)(MAX_VEL * (norm_x - norm_y));
    speed->right_wheel = (int)(MAX_VEL * (- norm_y - norm_x));

    speed->left_wheel = round_val(speed->left_wheel, ROUND_VEL);
    speed->right_wheel = round_val(speed->right_wheel, ROUND_VEL);

    // Clamp the velocities to the range -MAX_VEL to MAX_VEL
    if (speed->left_wheel > MAX_VEL) speed->left_wheel = MAX_VEL;
    if (speed->left_wheel < -MAX_VEL) speed->left_wheel = -MAX_VEL;
    if (speed->right_wheel > MAX_VEL) speed->right_wheel = MAX_VEL;
    if (speed->right_wheel < -MAX_VEL) speed->right_wheel = -MAX_VEL;
}


#define JS_EVENT_SIZE (sizeof(struct js_event))

volatile sig_atomic_t keep_running = 1;
struct js_event last_event_type2_num0;
struct js_event last_event_type2_num1;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void sigint_handler(int signum) {
    keep_running = 0;
}

void* read_joystick(void* arg) {
    int js = *(int*)arg;
    struct js_event event;

    while (keep_running) {
        ssize_t bytes = read(js, &event, JS_EVENT_SIZE);

        if (bytes == -1) {
            if (errno != EAGAIN) {
                perror("Error reading joystick event");
                break;
            }
        } else if (bytes == JS_EVENT_SIZE) {
            pthread_mutex_lock(&lock);
            if (event.type == 2 && (event.number == 0 || event.number == 1)) {
                if (event.number == 0) {
                    last_event_type2_num0 = event;
                } else if (event.number == 1) {
                    last_event_type2_num1 = event;
                }
            }
            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}

int main(int argc, const char** argv) {
    if (argc<3) 
    {
      printf("client.elf <serial_file> <baudrate>\n");
      exit(EXIT_FAILURE);
    }

    const char* serial_device=argv[1];
    int baudrate=atoi(argv[2]);

    int serial_fd = serial_open(serial_device);
    if (serial_fd == -1) {
      perror("serial_open");
      exit(EXIT_FAILURE);
    }
    serial_set_interface_attribs(serial_fd, baudrate, 0);
    serial_set_blocking(serial_fd, 1);



    const char *device = "/dev/input/js0";
    int js = open(device, O_RDONLY | O_NONBLOCK);

    if (js == -1) {
        perror("Could not open joystick device");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, sigint_handler);

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, read_joystick, &js) != 0) {
        perror("Could not create thread");
        close(js);
        exit(EXIT_FAILURE);
    }

    target_speed_t speed = {
        .left_wheel = 0,
        .right_wheel = 0
    };
    while (keep_running) {
        sleep(1);

        pthread_mutex_lock(&lock);
        // printf("Last event for type 2, number 0: time %u, value %d, type %u, number %u\n",
        //        last_event_type2_num0.time, last_event_type2_num0.value, last_event_type2_num0.type, last_event_type2_num0.number);
        // printf("Last event for type 2, number 1: time %u, value %d, type %u, number %u\n",
        //        last_event_type2_num1.time, last_event_type2_num1.value, last_event_type2_num1.type, last_event_type2_num1.number);
        mapJoystickToWheels(last_event_type2_num0.value, last_event_type2_num1.value, &speed);
        send_speed(serial_fd, &speed);
        printf("Left wheel: %d, Right wheel: %d\n", speed.left_wheel, speed.right_wheel);
        pthread_mutex_unlock(&lock);
    }

    pthread_join(thread_id, NULL);
    close(js);
    close(serial_fd);

    printf("Program terminated gracefully.\n");
    return 0;
}
