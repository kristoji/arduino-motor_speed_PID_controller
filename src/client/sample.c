/**************************************************\
 * @file sample.c                         
 *
 * @brief Implementation of functions to get samples
\**************************************************/

#include "sample.h"

void* sample(void* arg)
{
    int fd = *(int*)arg;


    packet_t packet;
    uint16_t prev_timestamp = -1;

    init_gnuplot();

    while (keep_running) 
    {
        receive_packet(fd, &packet);
        printf("received x: %f, y: %f, theta: %f, v: %f, w: %f\n", packet.x, packet.y, packet.theta, packet.v, packet.w);

        plot_with_gnuplot(packet.x, packet.y, packet.theta, packet.v, packet.w);
    }

    close_gnuplot();
} 

void receive_packet(int fd, packet_t* packet)
{
    // uint8_t buf[sizeof(packet_t)];
    // int read_bytes = 0;

    // do  
    // {
    //     int n = read(fd, buf + read_bytes, sizeof(packet_t) - read_bytes);
    //     read_bytes += n;
    // } while (read_bytes < sizeof(packet_t));

    // memcpy(packet, buf, sizeof(packet_t));

    char buf[100] = {0};
    int n;
    int read_bytes = 0;
    do
    {
        n = read(fd, buf + read_bytes, 100-read_bytes);
        read_bytes += n;
    } while (read_bytes < 100 && buf[read_bytes-1] != '\n');

    if (n>0)
    {
        sscanf(buf, "x: %f, y: %f, theta: %f, v: %f, w: %f", &packet->x, &packet->y, &packet->theta, &packet->v, &packet->w);
    }
    else
    {
        exit(1);
    }

}