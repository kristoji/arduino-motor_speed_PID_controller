#include "client.h"

void sigint_handler(int signum) {
    keep_running = 0;
}


int main(int argc, const char** argv) {
    if (argc<3) 
    {
      printf("client.elf <serial_file> <baudrate> <?debug>\n");
      exit(EXIT_FAILURE);
    }

    if (argc == 4) 
    {
        debug = atoi(argv[3]);
    }

    int serial_fd = serial_init(argv[1], atoi(argv[2]));
    int js_fd = check_open(DEV_JS);

    signal(SIGINT, sigint_handler);

    pthread_t js_thread_id, plot_thread_id;
    if (pthread_create(&js_thread_id, NULL, read_joystick, &js_fd) != 0 ||
        pthread_create(&plot_thread_id, NULL, sample, &serial_fd) != 0) 
    {
        perror("Could not create thread");
        close(js_fd);
        close(serial_fd);
        exit(EXIT_FAILURE);
    }


    while (keep_running) 
    {
        sleep(DELAY_SEND_CONTROL);

        pthread_mutex_lock(&lock);

        mapJoystickToWheels(last_event_x.value, last_event_y.value, &speed);
        send_speed(serial_fd, &speed);

        pthread_mutex_unlock(&lock);
        // printf("Left wheel: %d, Right wheel: %d\n", speed.left_wheel, speed.right_wheel);
    }


    pthread_join(js_thread_id, NULL);
    pthread_join(plot_thread_id, NULL);
    close(js_fd);
    reset_arduino(serial_fd);
    close(serial_fd);
    printf("Program ended\n");

    return 0;
}
