/**************************************************\
 * @file jstick.c                         
 *
 * @brief Implementation of functions to handle joystick
\**************************************************/
#include "jstick.h"

/**************************************************\
 * @brief saves the last joystick event on x axis
\**************************************************/
struct js_event last_event_x;
/**************************************************\
 * @brief saves the last joystick event on y axis
\**************************************************/
struct js_event last_event_y;

int check_open(const char* name)
{
    int fd = open(name, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("Could not open joystick device");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void send_speed(int fd, target_speed_t* speed)
{
    uint8_t sent_bytes = 0;
    uint8_t* ptr = (uint8_t*) speed;
    
    do
    {
        int n = write(fd, ptr + sent_bytes, sizeof(target_speed_t) - sent_bytes);
        sent_bytes += n;
    } while (sent_bytes < sizeof(target_speed_t));
}

int16_t round_val(int16_t value, int16_t round)
{
    int16_t remainder = value % round;

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

int16_t clamp(int16_t value, int16_t abs_max)
{
    if (value > abs_max) return abs_max;
    if (value < -abs_max) return -abs_max;
    return value;
}

void mapJoystickToWheels(int x, int y, target_speed_t * speed) 
{
    float norm_x = x / 32767.0;
    float norm_y = y / 32767.0;

    speed->left_wheel = (int)(MAX_VEL * (norm_x - norm_y));
    speed->right_wheel = (int)(MAX_VEL * (- norm_y - norm_x));

    speed->left_wheel = round_val(speed->left_wheel, ROUND_VEL);
    speed->right_wheel = round_val(speed->right_wheel, ROUND_VEL);

    speed->left_wheel = clamp(speed->left_wheel, MAX_VEL);
    speed->right_wheel = clamp(speed->right_wheel, MAX_VEL);
}


void* read_joystick(void* arg)
{
    int js = *(int*)arg;
    struct js_event event;

    while (keep_running) 
    {
        ssize_t bytes = read(js, &event, JS_EVENT_SIZE);

        if (bytes == -1) 
        {
            if (errno != EAGAIN) 
            {
                perror("Error reading joystick event");
                break;
            }
        } 
        else if (bytes == JS_EVENT_SIZE) 
        {
            pthread_mutex_lock(&lock);
            if (event.type == 2 && (event.number == 0 || event.number == 1)) 
            {
                if (event.number == 0) 
                {
                    last_event_x = event;
                } 
                else if (event.number == 1) 
                {
                    last_event_y = event;
                }
            }
            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}
