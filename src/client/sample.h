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

typedef struct __attribute__((packed)) packet_s
{
    float x;
    float y;
    float theta;
    float v;
    float w;
} packet_t;


void* sample(void* arg);

void receive_packet(int fd, packet_t* packet);


