/**************************************************\
 * @file sample.c                         
 *
 * @brief Implementation of functions to get samples
\**************************************************/

#include "sample.h"

uint8_t debug = 0;

void* sample(void* arg)
{
    int fd = *(int*)arg;

    // clear the serial buffer
    uint8_t buf[100];
    int n = read(fd, buf, 100);

    packet_t packet = {0};
    uint16_t prev_timestamp = -1;

    init_gnuplot();

    while (keep_running) 
    {
        if (debug)
        {
            printf("received x: %f, y: %f, theta: %f, v: %f, w: %f\n", packet.x, packet.y, packet.theta, packet.v, packet.w);
        }

        plot_with_gnuplot(packet.x, packet.y, packet.theta, packet.v, packet.w);
        
        receive_packet(fd, &packet);
    }

    close_gnuplot();
} 

void receive_packet(int fd, packet_t* packet)
{
    uint8_t buf[sizeof(packet_t)];
    int read_bytes = 0;

    do  
    {
        int n = read(fd, buf + read_bytes, sizeof(packet_t) - read_bytes);
        read_bytes += n;
    } while (read_bytes < sizeof(packet_t) && keep_running);

    memcpy(packet, buf, sizeof(packet_t));
}