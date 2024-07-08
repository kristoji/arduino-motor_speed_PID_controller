/**************************************************\
 * @file sample.h                         
 *
 * @brief Header for sampling functions
\**************************************************/

#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "plot.h"

extern volatile sig_atomic_t keep_running;

/**************************************************\
 * @brief Represents a packet of data to receive
 * 
 * @param x The x value of the robot
 * @param y The y value of the robot
 * @param theta The angle of the robot
 * @param v The linear velocity of the robot
 * @param w The angular velocity of the robot
\**************************************************/
typedef struct __attribute__((packed)) packet_s
{
    float x;
    float y;
    float theta;
    float v;
    float w;
} packet_t;

/**************************************************\
 * @brief Thread function to handle the received data
 * 
 * @param arg A pointer to the file descriptor to read from
\**************************************************/
void* sample(void* arg);

/**************************************************\
 * @brief Receives a packet from the Arduino
 * 
 * @param fd The file descriptor to read from
 * @param packet The packet to read into
\**************************************************/
void receive_packet(int fd, packet_t* packet);


