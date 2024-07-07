/**************************************************\
 * @file plot.h                         
 *
 * @brief Header for plotting functions
\**************************************************/

#pragma once
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define M_PI 3.14159265358979323846
#include <math.h>



/**************************************************\
 * @brief Initializes the gnuplot process
\**************************************************/
void init_gnuplot();

/**************************************************\
 * @brief Closes the gnuplot process
\**************************************************/
void close_gnuplot();

/**************************************************\
 * @brief Plots the values with gnuplot
 * 
 * @param values The values to plot
\**************************************************/
void plot_with_gnuplot(float x, float y, float theta, float v, float w);