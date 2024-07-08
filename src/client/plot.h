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
 * @brief Initializes the gnuplot pipe
\**************************************************/
void init_gnuplot();

/**************************************************\
 * @brief Closes the gnuplot pipe
\**************************************************/
void close_gnuplot();

/**************************************************\
 * @brief Plots the values with gnuplot
 * 
 * @param x The x value to plot
 * @param y The y value to plot
 * @param theta The angle to plot
 * @param v The linear velocity to plot
 * @param w The angular velocity to plot
\**************************************************/
void plot_with_gnuplot(float x, float y, float theta, float v, float w);